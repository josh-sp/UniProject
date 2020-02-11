#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);  //pins:RS,E,D4,D5,D6,D7

const int x1 = A0;   //x-axis of analog1
const int y1 = A1;   //y-axis of analog1
const int s1 = A2;   //switch of analog1
const int x2 = A3;   //x-axis of analog2
const int s2 = A5;   //switch of analog2
int xVal1, yVal1, sVal1;  //values assigned later
int xVal2, sVal2;

byte steps[3];
int stepsB, stepsS, stepsE = 0;
bool grip = false;  //gripper closed?

byte col1 = 2;   //1 for one direction
byte col2 = 2;   //2 for still
byte col3 = 2;   //3 for other direction
                 //if 0 is recieved there is no connection
byte data[3];       //used to create data for transmission
void setup() {
  lcd.begin(16, 2);     //LCD
  Serial.begin(9600);   //LoRa Baud rate (any should work)
}

void loop() { 
  xVal1 = analogRead(x1);   //read inputs
  yVal1 = analogRead(y1);
  sVal1 = analogRead(s1);
  xVal2 = analogRead(x2);
  sVal2 = analogRead(s2);

  readA1();
  readA2();

  writeLoRa();

  lcd.clear();
  lcd.print("B:" + String(stepsB));
  lcd.setCursor(8,0);
  lcd.print("S:" + String(stepsS));
  lcd.setCursor(0,1);
  lcd.print("E:" + String(stepsE));
  delay(50);
}

void readA1(){
   switch(xVal1){
    case 600 ... 1024:   //baseR
     col1 = 1; 
     stepsB +=1; break;
    case 0 ... 400:         //baseL
     col1 = 2;
     stepsB -=1; break;
    default:
     col1 = 3; break;
  }  
  switch(yVal1){
   case 600 ... 1024:    //shoulderR
    col2 = 1;
    stepsS +=1; break;
   case 0 ... 400:         //shoulderL
    col2 = 2;
    stepsS -=1; break;
   default:
    col2 = 3; break;
  }
  //ADD SWITCH READ HERE!!  
}

void readA2(){
  switch(xVal2){
    case 600 ... 1024:
     col3 = 1;
     stepsE +=1; break;
    case 0 ... 400:    
     col3 = 2;
     stepsE -=1; break;
    default:
     col3 = 3; break;
  }
  //NO Y AXIS FOR A2
  //ADD SWITCH READ HERE!!  
}

void writeLoRa(){   
  data[0] = col1;   //order doesnt matter
  data[1] = col2;   //just read the right way around on the other side
  data[2] = col3;
  Serial.write(data, 3);  
}
