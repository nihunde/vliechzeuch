#!/usr/bin/env python3

import sys
import os
import subprocess
import arduino as ard
import time
from typing import Optional


ATR_OK = ard.AtrMessage.OK()


def assert_eq(actual, expected):
    if actual != expected:
        print(f"Expected {expected!r}, got {actual!r}")


def assert_in(actual, expected_list):
    if actual not in expected_list:
        print(f"Expected any of {expected_list!r}, got {actual!r}")


class CallController(object):
    KINDS = ("commands", "status")

    def __init__(self, pipes, identity):
        self.ended_call = False
        self.commands, self.status = pipes

        self.commands.write(f"identity {identity}\n")
        assert_eq(self.status.readline(), "online")

    def _handle_call(self):
        while not self.ended_call:
            self.commands.write("continue_call\n")
            if self.status.readline() == "ended_call":
                self.ended_call = True
                break
            yield "in call"
        yield "ended"

    def call(self, number: str):
        self.commands.write(f"call {number}\n")
        yield self.status.readline()  # either "success" or "failure"
        yield all self._handle_call()

    def accept_incoming(self):
        self.commands.write("accept_incoming\n")
        return self._handle_call()

    def decline_incoming(self):
        self.commands.write("decline_incoming\n")

    def end_call(self):
        self.ended_call = True
        self.commands.write("end_call\n")

    def get_incoming(self) -> Optional[CallUpdate]:
        self.commands.write("get_incoming\n")
        out = self.status.readline()
        if out == "none":
            return None
        else:
            return out[len("some "):]


current_directory = os.getcwd()

options = [
    "8795@hg.eventphone.de",
    "/dev/ttyUSB0",
    current_directory,
]

HELP = f"""
./controller.py caller_identity arduino_addr working_directory
Each of those options can be omitted, in which case the default will be taken.
Omission can happen (except for simply not supplying enough arguments, which also works) either via
    using an empty argument (via e.g. "") or "-".
The default values are:
      * caller_identity = {options[0]!r} ("@hg.eventphone.de")
      * arduino_addr = {options[1]!r} ("/dev/" can be ommitted)
      * working_directory = the current working directory
IMPORTANT: The working_directory has to contain a directory named "sip" with the appropriate
    configuration
"""

if "-h" in sys.argv[1:]:
    print(HELP.strip())
    os.exit(0)

for idx, arg in enumerate(sys.argv[1:]):
    if arg not in ("", "-"):
        options[idx] = arg

caller_identity = options[0] if "@" in options[0] else f"{options[0]}@hg.eventphone.de"
arduino_addr = options[1] if options[1].startswith("/") else f"/dev/{options[1]}"
working_directory = options[2] if options[2].endswith("/") else f"{options[2]}/"

# Create the named pipes to allow for communication with the call controller
pipe_names = [f"{working_directory}/.call-{i}.pipe" for i in CallController.KINDS]
for i in pipe_names:
    os.mkfifo(i)

# Start the call controller
script_directory = os.path.dirname(os.path.realpath(__file__))
calling_process = subprocess.Popen([
    "/usr/bin/python2",
    f"{script_directory}/calling.py",
    working_directory
])
calling = CallController(pipe_names, caller_identity)

# Connect to the arduino
arduino = ard.ArduinoPart(arduino_addr)

def eliminate_error(val):
    try:
        error = val.error()
        print(f"ARDUINO ERROR: {error}")
        return ard.AtrMessage.OK()
    except:
        return val

def ended_call(number, start_time):
    diff_sec_full = time.time() - start_time if start_time is not None else 0
    diff_sec = diff_sec_full % 60
    diff_min = diff_sec_full // 60
    assert_eq(ard.RtaMessage.END_CALL(number, f"{diff_min}:{diff_sec}"))

def maybe_ended_call(msg, number, start_time):
    if eliminate_error(msg) == ard.AtrMessage.CALL_END:
        calling.end_call()
        ended_call(number, start_time)
        return True
    return False

def in_call(status, number):
    call_start_time = time.time()
    while next(status) == "in call":
        if maybe_ended_call(arduino.step(ard.RtaMessage.ACTIVE_CALL), number, call_start_time):
            return
    ended_call(number, call_start_time)

def dial_number(number):
    # TODO: Easter egg
    status = calling.call(number)
    if maybe_ended_call(arduino.step(ard.RtaMessage.DIALLING), number, None):
        return
    if next(status) != "success":
        assert_in(
            arduino.step(ard.RtaMessage.END_CALL(b"Failed call", b"0:0")),
            (ATR_OK, ard.AtrMessage.CALL_END)
        )
        return
    if maybe_ended_call(assert_eq(arduino.step(ard.RtaMessage.CALLING), ATR_OK), number, None):
        return
    in_call(status, number)

calling_number = b""

while True:
    response = arduino.step()
    if response:
        easteregg_error = "Arduino tried to use easteregg controls outside the easteregg"
        eliminate_error(response).match(
            ok=lambda: None,
            error=lambda e: None,  # Can't happen
            dial_number=lambda n: dial_number(n.decode("ascii")),
            accept=lambda: in_call(calling.accept_incoming(), calling_number),
            decline=calling.decline_incoming(),
            call_end=lambda: print("Arduino ended call although no call is in process"),
            easteregg_up=lambda: print(easteregg_error),
            easteregg_down=lambda: print(easteregg_error),
        )
    else:
        incoming = calling.get_incoming()
        if incoming is not None:
            try:
                encoded = incoming.encode("ascii")
            except:
                encoded = b"{Couldn't display}"
            assert_eq(arduino.step(ard.RtaMessage.CALL_INCOMING(encoded)), ATR_OK)
