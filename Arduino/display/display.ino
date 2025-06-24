#include <LiquidCrystal.h>

LiquidCrystal lcd(46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26);
void setup() {
  pinMode(52, 0);
  pinMode(50, 1);
  pinMode(25,1);
  pinMode(22, 0);


}

void loop() {
  lcd.begin(16,1);
  lcd.print("hello, world!");
}
