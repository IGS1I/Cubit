#include <LiquidCrystal.h>
#include <DHT.h>

/* Create LCD object with Arduino pin mapping:
  RS, E, D4, D5, D6, D7 - lcd object
  GNDs - VSS, RW (read/write off since only reading), K
  5Vs - VDD, A

*/
LiquidCrystal lcd(27, 26, 25, 24, 23, 22);

#define DHTPIN 2       
#define DHTTYPE DHT11  // DHT11 or DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  // ~9600 bps
  Serial.begin(9600);
  dht.begin();

  analogWrite(28, 0); // write contrast val for LCD

  // Initialize LCD with 16 columns, 2 rows
  lcd.begin(16, 2);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  // Print text on first row
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.print("°C");
  lcd.setCursor(0, 1); // Move to second row and print more text
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  
}
