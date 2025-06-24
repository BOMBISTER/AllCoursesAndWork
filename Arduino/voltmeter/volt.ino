#include <SevSeg.h>

SevSeg sevseg;

#define battery A0

#define a 5
#define b 6
#define c 7
#define d 8
#define e 9
#define f 10
#define g 11
#define dp 2

#define d1 A5
#define d2 A2
#define d3 A3
#define d4 A4
//0.009 6.798 (-0.45)
void setup() {
  Serial.begin(9600);
  byte digitNumber = 4;
  byte digitPins[] = {d4, d3, d2, d1};
  byte segPins[] = {a, b, c, d, e, f, g, dp};
  sevseg.begin(COMMON_CATHODE, digitNumber, digitPins, segPins); 
  sevseg.setBrightness(100);
  
}

void displayShow(float volt){
  
  int intVolts = volt;
  
  int floatVolts = (volt - intVolts) * 100;
  
  sevseg.setNumber(intVolts * 100 + floatVolts, 2); 
  sevseg.refreshDisplay();

}

void loop() {
  // int batteryVal = analogRead(battery);
  // float voltage = batteryVal * (5.08 / 1023.0);
  // displayShow(voltage);
  int y;
  y = analogRead(battery);
  Serial.println(y*(5.08/1023.0));
}
