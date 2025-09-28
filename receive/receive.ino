#include <SoftwareSerial.h>
// RX, TX (Arduino’s perspective)
SoftwareSerial esp(0, 1); // pin 10 <- ESP TX, pin 11 -> ESP RX (unused if one-way)

void setup() {
  Serial.begin(115200);   // for PC Serial Monitor
  esp.begin(115200);      // must match ESP baud
}

void loop() {
  if (esp.available()) {
    int c = esp.read();
    Serial.write(c);  // forward to Serial Monitor
  }
  delay(1000);
}
