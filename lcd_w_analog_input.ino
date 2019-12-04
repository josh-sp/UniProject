#include <LiquidCrystal.h> //library for LCD
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

const int x = A0;
const int y = A1;
//const int sw = A2;
String xDir;
String yDir;

void setup() {
  lcd.begin(16, 2); //number of cols and rows
  Serial.begin(9600);
}

void loop() {
  int xVal = analogRead(x);     //Read inputs
  int yVal = analogRead(y);
 // int btn = analogRead(sw);

  switch(xVal){
    case 600 ... 1024:
      xDir = "Right";
      break;
    case 0 ... 500:
      xDir = "Left";
      break;
    default:
      xDir = "Center";
      break;
  }
    switch(yVal){
    case 600 ... 1024:
      yDir = "Down";
      break;
    case 0 ... 500:
      yDir = "Up";
      break;
    default:
      yDir = "Center";
      break;
  }

  lcd.clear();
  lcd.setCursor(0,0); //set cursor to col1, line 1
  lcd.print(xDir);
  lcd.setCursor(0,1); //go down 1 line
  lcd.print(yDir);
  delay(100);
  
}
