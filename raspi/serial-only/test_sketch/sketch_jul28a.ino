String last_message;

void setup() {
  Serial.begin(9600);
  last_message = "";
  delay(3000);
  Serial.setTimeout(500);
}

void next_msg() {
  last_message = Serial.readStringUntil('\r');
  if (last_message.length() > 0) {
    while (Serial.available() == 0) { delay(1); }
    Serial.read();  // discard the \n
  }
}

void test(String s, bool wait=true) {
  Serial.println(s);
  if (wait) {
    next_msg();
    if (last_message.length() > 0 && last_message[0] != 'A') {
      // we encountered a race condition. Wonderful.
      Serial.println("brace " + last_message);
    }
  }
  next_msg();
  if (last_message.length() > 0) {
    Serial.println("breceived " + last_message);
  }
}

void loop() {
  test("a", false);  // ok
  test("bthis is an error");
  test("cnumber to dial");
  test("d");  // accept
  test("e");  // decline
  test("f");  // call end
  test("g");  // easteregg up
  test("h");  // easteregg down
}
