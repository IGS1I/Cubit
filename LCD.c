#include <Adafruit_LiquidCrystal.h>

//int firstLED = 12;
//int secondLED = 13;
//int powerLED = 11;

int powerSwitch = 0;

Adafruit_LiquidCrystal lcd(0);

void blinkLED(int blinkDelay)
{
  digitalWrite(firstLED, HIGH);
  digitalWrite(secondLED, HIGH);
  delay(blinkDelay);
  digitalWrite(firstLED, LOW);
  digitalWrite(secondLED, LOW);
  delay(blinkDelay);
}

float readUltrasonicSensor(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  
  return pulseIn(echoPin, HIGH);
}


void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(firstLED, OUTPUT);
  pinMode(secondLED, OUTPUT);
  pinMode(powerSwitch, INPUT);
}

void loop()
{
  lcd.setBacklight(LOW);
  while(powerSwitch ==0)
  {
    powerSwitch = digitalRead(10);
  }

  
  lcd.begin(16,2);
  lcd.setBacklight(HIGH);
  
  int cm = 0.01723*readUltrasonicSensor(7,7);
  int inches = (cm/2.54);
  if(inches > 0 && inches <= 40)
  {
    lcd.clear();
    lcd.setCursor(1,0);
  lcd.print("DANGER");
    blinkLED(200);
    
  }
  else if(inches > 40 && inches <= 70)
  {
    lcd.clear();
    lcd.setCursor(1,0);
  lcd.print("WARNING");
    blinkLED(500);
    
  }
  else if(inches > 70 && inches <= 100)
  {
    lcd.clear();
    lcd.setCursor(1,0);
  lcd.print("ALERT");
    blinkLED(750);
    
  }
  else{
    lcd.clear();
    lcd.setCursor(1,0);
  lcd.print("CAUTION");
    blinkLED(1000);
  }
}
    
