#include <LiquidCrystal.h>
#include <DHT.h>
#include <SoftwareSerial.h>

/* Create LCD object with Arduino pin mapping:
  RS, E, D4, D5, D6, D7 - lcd object
  GNDs - VSS, RW (read/write off since only reading), K
  5Vs - VDD, A

  For Serial: TX->RX && RX->TX
*/
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define DHTPIN 8
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  // ~9600 bps
  Serial.begin(115200);
  dht.begin();

  analogWrite(28, 0);

  lcd.begin(16, 2);
}
void loop() {
    // Check for incoming messages
    if (Serial.available()) {
      String message = Serial.readString();
      message.trim();

      // Display message on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(message.substring(0, 16)); // First line (16 chars max)
      if (message.length() > 16) {
        lcd.setCursor(0, 1);
        lcd.print(message.substring(16, 32)); // Second line
      }

      delay(2000); // Show message for 2 seconds
    }

  // Continue will usual display
  float humidity = dht.readHumidity();
  int temperature = round(dht.readTemperature());

  delay(1000);
  lcd.setCursor(0, 0);
  lcd.clear();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error: Failed to read from DHT11 sensor!");
    lcd.print("Error: Sensor");
    return;
  }

  // Printing
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C / ");
  lcd.print(((9/5) *  temperature) + 32);
  lcd.print("F");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
}
