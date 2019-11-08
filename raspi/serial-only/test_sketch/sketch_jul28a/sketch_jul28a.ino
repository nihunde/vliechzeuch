#include "/home/cklaeren/code/vliechzeuch/arduino/main/main/raspi.h"

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.setTimeout(500);
  init_raspi();
}

String next_msg() {
  String msg = "";
  while (true) {
    while (Serial.available() == 0) { delay(1); }
    char c = Serial.read();
    if (c == '\r') {
      // Do nothing
    } else if (c == '\n') {
      return msg;
    } else {
      msg += c;
    }
  }
  return msg;
}

void test(String s) {
  while (true) {
    String msg = next_msg();
    if (msg == "Z") {
      Serial.print(s + "\r\n");
      return;
    } else {
      Serial.print("brec:" + msg + "\r\n");
    }
  }
}

void loop() {
  switch (LAST_COMMAND_KIND) {
    case RTA_OK:
      send(ATR_OK);
      break;
    case RTA_IDLE:
      send(ATR_ERROR, "Idle!");
      break;
    case RTA_CALL_INCOMING:
      send(ATR_ERROR, String("incoming: ") + CALLER);
      break;
    case RTA_DIALLING:
      send(ATR_ERROR, "Dialling...");
      break;
    case RTA_CALLING:
      send(ATR_ERROR, "Calling...");
      break;
    case RTA_ACTIVE_CALL:
      send(ATR_ERROR, "In a call!");
      break;
    case RTA_END_CALL:
      send(ATR_ERROR, String("ended, ") + DURATION + String(" with ") + CALLER);
      break;
    case RTA_EASTEREGG:
      send(ATR_ERROR, "it's easter, bitches! " + String(OWN_PADDLE_LOCATION) + " " + String(ENEMY_PADDLE_LOCATION) + " " + String(BALL_X) + " " + String(BALL_Y));
      break;
    case RTA_DEBUG:
      send(ATR_ERROR, String("debugging: ") + RTA_DEBUG_INFO);
      break;
    case RTA_SCREEN_LOCK:
      send(ATR_ERROR, "locked...");
      break;
    case RTA_NUP:
      send_nothing();
      break;
  }
}
