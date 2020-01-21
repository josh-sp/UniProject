#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);  //pins:RS,E,D4,D5,D6,D7

const int x1 = A0;   //x-axis of analog1
const int y1 = A1;   //y-axis of analog1
const int s1 = A2;   //switch of analog1
const int x2 = A3;   //x-axis of analog2
const int y2 = A4;   //y-axis of analog2
const int s2 = A5;   //switch of analog2
int xVal1;  //values assigned later
int yVal1;
int sVal1;
int xVal2;
int yVal2;
int sVal2;

int stepsB = 0;     //base # of steps
int stepsS = 0;     //shoulder # of steps
int stepsE = 0;     //elbow # of steps
bool grip = false;  //gripper closed?

int col1 = 1;   //motor 1 arr y index
int col2 = 1;   //motor 2 arr y index
int col3 = 1;   //motor 3 arr y index
int cols[2];    //used to construct data packet for transmission
void setup() {
  lcd.begin(16, 2);     //LCD
  Serial.begin(9600);   //LoRa Baud rate (any should work)
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

  writeLoRa();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("B:" + String(stepsB));
  lcd.setCursor(8,0);
  lcd.print("S:" + String(stepsS));
  //lcd.setCursor(0,1);
  //lcd.print(stepsE);

  delay(50);
}

void readA1(){
    switch(xVal1){
    case 600 ... 1024:   //baseR
     if(stepsB < 100){
      if(col1 >= 3){col1=0;}
      else{col1 +=1;}
      stepsB +=1;        
     }
    break;
   case 0 ... 400:         //baseL
     if(stepsB > -100){
      if(col1==0){col1=3;}
      else{col1 -=1;}      
      stepsB -=1;        
     }
    break;
  }
  
  switch(yVal1){
   case 600 ... 1024:    //shoulderR
     if(stepsS > -100){
      if(col2 >= 3){col2=0;}
      else{col2 +=1;}
      stepsS -=1;        
     }
    break;
   case 0 ... 400:         //shoulderL
     if(stepsS < 100){
      if(col2==0){col2=3;}
      else{col2 -=1;}
      stepsS +=1;        
     }
    break;
  }
  //ADD SWITCH READ HERE!!  
}

void readA2(){
  switch(xVal2){
    case 600 ... 1024:   //elbowR
      if(stepsE < 100){
       if(col3 >= 3){col3=0;}
       else{col3 +=1;}
       stepsE +=1;        
      }
    break;
    case 0 ... 400:         //elbowL
     if(stepsE > -100){
      if(col3==0){col3=3;}
      else{col3 -=1;}
      stepsE -=1;        
     }
    break;
  }
  //ADD SWITCH READ HERE!!  
}

void writeLoRa(){ //using x,y,z for col1,2,3
  switch(col1){
    case 0:
     Serial.write(10);
    break;
    case 1:
     Serial.write(20);
    break;
    case 2:
     Serial.write(30);
    break;
    case 3:
     Serial.write(40);
    break; 
  }
}
