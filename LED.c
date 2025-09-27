#include <LiquidCrystal.h>

// Create LCD object with Arduino pin mapping:
// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(24, 26, 23, 25, 27, 29);

void setup() {

  analogWrite(22, 0);

  // Initialize LCD with 16 columns, 2 rows
  lcd.begin(16, 2);

  // Print text on first row
  lcd.print("Hello, World!");

  // Move to second row and print more text
  lcd.setCursor(0, 1);
}

void loop() {
  // Example: simple counter
  static int count = 0;
  lcd.setCursor(0, 1);   // Second row
  lcd.print("Count: ");
  lcd.print(count++);
  delay(1000);
}
