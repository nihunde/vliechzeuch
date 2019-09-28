import sys
import traceback

import arduino as ard

arduino = ard.ArduinoPart("/dev/ttyUSB0" if len(sys.argv) <= 1 else sys.argv[1])

kind_names = [i for i in dir(ard.RtaMessage) if i.upper() == i]
locals().update({i.lower(): getattr(ard.RtaMessage, i) for i in kind_names})

while True:
    try:
        command = eval(input(), locals(), globals())
        print(repr(arduino.step(command)))
    except Exception as e:
        print("\n".join(traceback.format_exception(*sys.exc_info())))
        print("possible messages: " + ", ".join(i.lower() for i in kind_names))
