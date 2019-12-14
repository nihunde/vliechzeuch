from application.notification import NotificationCenter, NotificationData, IObserver
from application.python import Null

from zope.interface import implements

from sipsimple.application import SIPApplication

from threading import Thread, Event


class Controller(Thread):
    KINDS = ("commands", "status")

    def __init__(self, pipes, application):
        Thread.__init__()
        self.commands, self.status = pipes
        self.application = application
        self.done = Event()
        self.start()

    def run(self):
        command = self.commands.readline()
        if command.startswith("identity "):
            self.done.clear()
            self.application.send(lambda app: app.start(command[len("identity "):], controller))
            self.done.wait()
            self.status.write("online\n")
        elif command == "continue_call":
            call_done = False  # TODO
            self.status.write("in_call\n" if call_done else "ended_call\n")
        elif command.startswith("call "):
            success = False  # TODO
            self.status.write("success" if success else "failure")
        elif command == "accept_incoming":
            # TODO. Doesn't send status update.
        elif command == "decline_incoming":
            # TODO. Doesn't send status update.
        elif command == "end_call":
            # TODO. Doesn't send status update.
        elif command == "get_incoming":
            incoming = None  # TODO
            self.status.write(f"some {incoming}" if incoming is not None else "none")


class Application(SIPApplication):
    def __init__(self):
        SIPApplication.__init__(self)
        self.notification_center = NotificationCenter()
        self.notification_center.add_observer(self)
        self.controller = Null

    def send(self, command):  # Hacky, but should work. The proper solution would probably to look how twisted works, but I honestly can't be fucking bothered
        self.handle_notification("send", command)

    def _NH_send(self, command):
        command(self)

    def start(self, controller, identity):
        SIPApplication.start(self, FileStorage(f"{sys.argv[1]}.sip"))
        self.controller = controller
        # TODO: Do identity shit
