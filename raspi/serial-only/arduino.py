from typing import Tuple, Optional
from enum import Enum, unique as unique_enum

import serial


@unique_enum
class AtrMessageType(Enum):
    OK = 0x61   # 'A'
    ERROR = 0x62
    DIAL_NUMBER = 0x63
    ACCEPT = 0x64
    DECLINE = 0x65
    CALL_END = 0x66
    EASTEREGG_UP = 0x67
    EASTEREGG_DOWN = 0x68


_MAX_LENGTH_ATR = {
    AtrMessageType.OK: 0,
    AtrMessageType.ERROR: 255,
    AtrMessageType.DIAL_NUMBER: 255,
    AtrMessageType.ACCEPT: 0,
    AtrMessageType.DECLINE: 0,
    AtrMessageType.CALL_END: 0,
    AtrMessageType.EASTEREGG_UP: 0,
    AtrMessageType.EASTEREGG_DOWN: 0,
}


@unique_enum
class RtaMessageType(Enum):
    OK = 0x41  # 'a'
    IDLE = 0x42
    CALL_INCOMING = 0x43
    DIALLING = 0x44
    CALLING = 0x45
    ACTIVE_CALL = 0x46
    END_CALL = 0x47
    EASTEREGG = 0x48
    DEBUG = 0x49
    SCREEN_LOCK = 0x4a


_MAX_LENGTH_RTA = {
    RtaMessageType.OK: 0,
    RtaMessageType.IDLE: 0,
    RtaMessageType.CALL_INCOMING: 255,
    RtaMessageType.DIALLING: 0,
    RtaMessageType.CALLING: 0,
    RtaMessageType.ACTIVE_CALL: 0,
    RtaMessageType.END_CALL: 255,
    RtaMessageType.EASTEREGG: 4,
    RtaMessageType.DEBUG: 255,
    RtaMessageType.SCREEN_LOCK: 0,
}


class ArduinoPart(object):
    def __init__(self, port: str, *, baudrate=9600):
        self.serial = serial.Serial(port, timeout=0, baudrate=baudrate)
        self._buffer = b""
        self._read_buffer = []

    def _receive_internal(self) -> Optional[Tuple[AtrMessageType, bytes]]:
        blocking = False

        while True:
            # this loop is escaped everywhere via a return, excluding
            # the case in which a bare `b'\r\n'` was sent.

            while b'\r\n' not in self._buffer:
                next_bytes = self.serial.read(256)
                # we need a maximum of 256 bytes either way.

                if len(next_bytes) == 0:  # there was nothing received
                    if not blocking:
                        return None  # we're done.
                else:
                    self._buffer += next_bytes
                    blocking = True

            current, self._buffer = self._buffer.split(b'\r\n', 1)

            if len(current) == 0:
                continue # read again
            
            try:
                command = AtrMessageType(current[0])
            except ValueError:
                raise ValueError(
                    f"Unknown message type ({current!r} was received)"
                )

            if len(current[1:]) > _MAX_LENGTH_ATR[command]:
                raise ValueError(
                    f"Excess data ({current[1:]!r}) was sent for {command!r} "
                    + f"({len(current[1:])} > {_MAX_LENGTH_ATR[command]}"
                )

            return command, current[1:]

    def _send_internal(self, command: RtaMessageType, data: bytes=b"") -> None:
        self.serial.write(bytes([command.value]) + data + b"\r\n")
        self.serial.flush()

        kept_message_types = [AtrMessageType.ERROR]
        
        if command == RtaMessageType.EASTEREGG:
            kept_message_types += [
                AtrMessageType.EASTEREGG_UP,
                AtrMessageType.EASTEREGG_DOWN
            ]

        self._read_buffer = [
            i for i in self._read_buffer if i[0] in kept_message_types
        ]

        if command not in [RtaMessageType.OK, RtaMessageType.DEBUG]:
            while True:
                msg = self._receive_internal()
                if msg is not None:
                    cmd, data = msg
                    if cmd == AtrMessageType.OK:
                        return
                    elif cmd in kept_message_types:
                        self._read_buffer.append((cmd, data))

    def _receive_all_new(self):
        while True:
            x = self._receive_internal()
            if x is None:
                return
            else:
                self._read_buffer.append(x)

    def send(self, command: RtaMessageType, data: bytes=b"") -> None:
        """
        `send` sends a message. This throws an error if supplied with
        incorrect arguments. This blocks the current command gets an ok.
        """

        if not isinstance(command, RtaMessageType):
            raise ValueError("Expected `command` to be `RtaMessageType`")
        if not isinstance(data, bytes):
            raise ValueError("Expected `data` to be `bytes`")
        if len(data) > _MAX_LENGTH_RTA[command]:
            raise ValueError(
                f"`data` ({data!r}) is longer than it's allowed to be for "
                + f"{command!r} ({len(data)} > {_MAX_LENGTH_RTA[command]})"
            )

        if command == RtaMessageType.OK:
            print("Sending unexpected OK. This is probably a mistake.")

        self._receive_all_new()  # to allow filtering
        self._send_internal(command, data)

    def receive(self) -> Optional[Tuple[AtrMessageType, bytes]]:
        if self._read_buffer:
            out = self._read_buffer.pop(0)
        else:
            out = self._receive_internal()

        if out is not None:
            if out[0] == AtrMessageType.OK:
                print("Sent unexpected OK. This is probably a mistake.")
            elif out[0] != AtrMessageType.ERROR:
                self._send_internal(RtaMessageType.OK)
        return out

    def close(self) -> None:
        self.serial.close()
