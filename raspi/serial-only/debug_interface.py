import threading
import sys
import time

import arduino as ard


arduino = ard.ArduinoPart("/dev/ttyUSB0" if len(sys.argv) <= 1 else sys.argv[1])

lock = threading.Lock()


class OutputThread(threading.Thread):
    def run(self):
        while True:
            try:
                lock.acquire()
                x = arduino.receive()
                lock.release()
                if x is not None:
                    if x[1]:
                        print(f"{x[0]!r}, {x[1]!r}")
                    else:
                        print(f"{x[0]!r}")
                time.sleep(0.1)
            except ValueError as e:
                print(f"An error occured while receiving: {e!r}")
                raise e

OutputThread().start()

while True:
    c = input().split(" ", 2)

    data = bytes(c[1], "utf8") if len(c) > 1 else b""

    command = None
    for i in ard.RtaMessageType:
        if c[0].lower() == i.name.lower():
            command = i
            break
    else:
        print("Unknown command.")
        print("The following are known: " + ", ".join(map(repr, ard.RtaMessageType)))

    if command:
        if data:
            print("Sending {command!r}, {data!r}")
        else:
            print("Sending {command!r}")

        try:
            arduino.send(command, data)
        except ValueError as e:
            print(f"An error occured while sending: {e!r}")
        else:
            print("Sent")
