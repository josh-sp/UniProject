#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);  //pins:RS,E,D4,D5,D6,D7

const int x1 = A0;   //x-axis of analog1
const int y1 = A1;   //y-axis of analog1
const int s1 = A2;   //switch of analog1
const int x2 = A3;   //x-axis of analog2
const int y2 = A4;   //y-axis of analog2
const int s2 = A5;   //switch of analog2
int xVal1, yVal1, sVal1;  //values assigned later
int xVal2, yVal2, sVal2;

byte stepsB, stepsS, stepsE = 100; //0-99 is backwards, 100-200 is forward
byte gripper = 0; //needs to be byte for wireless comms

byte dataOut[4];      //used to create data for transmission
void setup() {
  lcd.begin(16, 2);     //LCD
  Serial.begin(9600);   //LoRa Baud rate (any should work. 9600:default)
  stepsB = 100;
  stepsS = 100;
  stepsE = 100;
}

void loop() { 
  xVal1 = analogRead(x1);   //read inputs
  yVal1 = analogRead(y1);
  sVal1 = analogRead(s1);
  xVal2 = analogRead(x2);
  yVal2 = analogRead(y2);
  sVal2 = analogRead(s2);

  readA1();
  readA2();
  
  lcd.clear();
  lcd.print("B:" + String(map(stepsB, 0, 200, -100, 100)));
  lcd.setCursor(8,0);
  lcd.print("S:" + String(map(stepsS, 0, 200, -100, 100)));
  lcd.setCursor(0,1);
  lcd.print("E:" + String(map(stepsE, 0, 200, -100, 100)));
  lcd.setCursor(8,1);
  lcd.print("G:" + String(gripper));
  delay(50);
}

void readA1(){
   switch(xVal1){ //base
    case 700 ... 1024:
    if(stepsB < 200){
      ++stepsB;
    } break;
    case 0 ... 300:
     if(stepsB > 0){
      --stepsB;
     } break;
  }  
  switch(yVal1){  //shoulder
   case 0 ... 300:
    if(stepsS  > 0){
      --stepsS;
    } break;
   case 700 ... 1024:
    if(stepsS < 200){
      ++stepsS;
    } break;
  }
  if(sVal1 == 0){
    gripper = !gripper;
    delay(100); //software debouncing
  }
}

void readA2(){  //elbow
  switch(xVal2){
    case 700 ... 1024:
     if(stepsE < 200){
      ++stepsE;
     } break;
    case 0 ... 300:    
     if(stepsE > 0){
      --stepsE;
     } break;
  }
  if(sVal2 == 0){ //Switch is pulled low
   writeLoRa();
   lcd.clear();
   lcd.print("Position Sent!");
   delay(1000);
  }
}

void writeLoRa(){ 
  dataOut[0] = stepsB;   //order doesnt matter
  dataOut[1] = stepsS;   //just read the right way around on the other side
  dataOut[2] = stepsE;
  dataOut[3] = gripper;
  Serial.write(dataOut, 4);  
}
