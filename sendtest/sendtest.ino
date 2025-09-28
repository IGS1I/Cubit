  void setup() {
    Serial.begin(115200);
    Serial.println("ESP8266 ready to send messages");
  }

  void loop() {
    Serial.println("Hello from ESP8266!");
    delay(3000);
  }