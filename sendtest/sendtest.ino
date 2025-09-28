#include <Arduino.h>
  void setup() {
    Serial.begin(9600);
    pinMode(3, OUTPUT); // Request pin
    digitalWrite(3, HIGH);
  }

  void requestSensorData() {
    // Brief interrupt pulse
    digitalWrite(3, LOW);
    delayMicroseconds(100); // Very short pulse
    digitalWrite(3, HIGH);

    // Wait for response with timeout
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) { // 1 second timeout
      if (Serial.available()) {
        String data = Serial.readString();
        Serial.println("Received: " + data);
        break;
      }
    }
  }
