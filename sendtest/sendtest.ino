#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT); // Request pin to Uno's pin 13
  digitalWrite(D1, HIGH);
  Serial.println("ESP8266 Sender Ready");
}

void loop() {
  Serial.println("Requesting sensor data...");
  requestSensorData();
  delay(5000); // Request every 5 seconds
}

void requestSensorData() {
  // Brief interrupt pulse
  digitalWrite(D1, LOW);
  delayMicroseconds(100); // Very short pulse
  digitalWrite(D1, HIGH);

  // Wait for response with timeout
  unsigned long startTime = millis();
  while (millis() - startTime < 1000) { // 1 second timeout
    if (Serial.available()) {
      String data = Serial.readString();
      data.trim();
      Serial.println("Received: " + data);

      // Parse temperature and humidity
      int commaIndex = data.indexOf(',');
      if (commaIndex > 0) {
        String temp = data.substring(0, commaIndex);
        String humid = data.substring(commaIndex + 1);
        Serial.println("Temperature: " + temp + "°C");
        Serial.println("Humidity: " + humid + "%");
      }
      break;
    }
  }
}
