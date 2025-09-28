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

volatile bool dataRequested = false;

void setup() {

  // ~9600 bps
  Serial.begin(9600);
  dht.begin();

  analogWrite(28, 0);

  lcd.begin(16, 2);

  // Set up interrupt on pin 2 for data request
  pinMode(13, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(13), requestData, FALLING);
}

void loop() {
  float humidity = dht.readHumidity();
  int temperature = round(dht.readTemperature());

  delay(1000);
  lcd.setCursor(0, 0);
  lcd.clear();

  if (isnan(humidity) || isnan(temperature)) {
    lcd.print("Error: Sensor");
    if (dataRequested) {
      Serial.println("ERROR");
      dataRequested = false;
    }
    return;
  }

  // Check if data was requested via interrupt
  if (dataRequested) {
    Serial.print(temperature);
    Serial.print(",");
    Serial.println(humidity);
    dataRequested = false;
  }

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

void requestData() {
  dataRequested = true;
}
