#include <LiquidCrystal.h>
#include <DHT.h>

/* Create LCD object with Arduino pin mapping:
  RS, E, D4, D5, D6, D7 - lcd object
  GNDs - VSS, RW (read/write off since only reading), K
  5Vs - VDD, A

*/
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define DHTPIN 8      
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  // ~9600 bps
  Serial.begin(9600);
  dht.begin();

  analogWrite(28, 0);

  lcd.begin(16, 2);
}

void loop() {
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

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

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
