from typing import Tuple, Optional

from adt import adt, Case

import serial


def log(x):
    print(f"[LOG]   {x!r}")
    return x


@adt
class RtaMessage(object):
    OK: Case
    IDLE: Case
    CALL_INCOMING: Case[bytes]  # number or name calling
    DIALLING: Case
    CALLING: Case
    ACTIVE_CALL: Case
    END_CALL: Case[bytes, bytes]  # number or name called and duration (preformatted). The first argument is not allowed to include a colon.
    EASTEREGG: Case[int, int, int, int]  # position own, position enemy, x, y
    DEBUG: Case[bytes]
    SCREEN_LOCK: Case


@adt
class AtrMessage(object):
    OK: Case
    ERROR: Case[bytes]
    DIAL_NUMBER: Case[bytes]
    ACCEPT: Case
    DECLINE: Case
    CALL_END: Case
    EASTEREGG_UP: Case
    EASTEREGG_DOWN: Case


class ArduinoPart(object):
    def __init__(self, port: str, *, baudrate=9600):
        self.serial = serial.Serial(port, timeout=None, baudrate=baudrate)

    def step(self, command: Optional[RtaMessage]=None) -> Optional[AtrMessage]:
        """
        Sends the given command (or NUP if none was supplied) to the Arduino.
        This blocks until the Arduino responds (usually with NUP, which would be mapped to `None`), this response is then returned.
        """
        def encode(command: Optional[RtaMessage]) -> bytes:
            if command is None:
                return b'\x5a'  # NUP
            else:
                return command.match(
                    ok=lambda: b'\x41',
                    idle=lambda: b'\x42',
                    call_incoming=lambda caller: b'\x43' + caller,
                    dialling=lambda: b'\x44',
                    calling=lambda: b'\x45',
                    active_call=lambda: b'\x46',
                    end_call=lambda caller, duration: b'\x47' + caller + b':' + duration,
                    easteregg=lambda p1, p2, bx, by: b'\x48' + bytes([p1, p2, bx, by]),
                    debug=lambda msg: b'\x49' + msg,
                    screen_lock=lambda: b'\x4a',
                )

        def decode(b: bytes) -> Optional[AtrMessage]:
            try:
                out, expected_max_length = {
                    0x61: lambda: (AtrMessage.OK(), 0),
                    0x62: lambda: (AtrMessage.ERROR(b[1:]), 255),
                    0x63: lambda: (AtrMessage.DIAL_NUMBER(b[1:]), 255),
                    0x64: lambda: (AtrMessage.ACCEPT(), 0),
                    0x65: lambda: (AtrMessage.DECLINE(), 0),
                    0x66: lambda: (AtrMessage.CALL_END(), 0),
                    0x67: lambda: (AtrMessage.EASTEREGG_UP(), 0),
                    0x68: lambda: (AtrMessage.EASTEREGG_DOWN(), 0),
                    0x7a: lambda: (None, 0),  # NUP
                }[b[0]]()
            except KeyError:
                raise ValueError(f"Unknown command kind: 0x{b[0]:x} (data was {b!r})")

            if len(b) - 1 > expected_max_length:
                raise ValueError(f"The given command ({out!r}) had unused data: {b[expected_max_length+1:]!r}")

            return out

        encoded = encode(command)
        if len(encoded) > 256:
            raise ValueError(f"Couldn't encode command, since the associated data was too long")
        if b'\r' in encoded or b'\n' in encoded:
            raise ValueError(f"Newline characters were present in the associated data for the command")

        self.serial.write(log(encoded + b"\r\n"))
        self.serial.flush()

        buf = self.serial.read(3)
        while b'\r' not in buf:
            buf += self.serial.read(2)
        if buf.endswith(b'\r'):
            buf += self.serial.read(1)

        if not buf.endswith(b'\r\n'):
            raise ValueError(f"The arduine sent a lone '\\r'. This error is potentially irrecoverable.")

        log(buf)

        return decode(buf[:-2].lstrip(b'\0'))

    def close(self) -> None:
        self.serial.close()
