#include <LiquidCrystal.h>
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);  //pins:RS,E,D4,D5,D6,D7
const int x = A0;
const int y = A1;
int m1[4] = {1, 0, 0, 0};   //m1= coil1: 6&7, coil2: 8&9
int m2[4] = {1, 0, 0, 0};   //m2= coil1: 10&11, coil2: 12&13
int stepsB = 0; //base
int stepsS = 0; //shoulder

void setup() {
  pinMode(6, OUTPUT);   //m1 pins
  pinMode(7, OUTPUT);    
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);  //m2 pins
  pinMode(11, OUTPUT);    
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  lcd.begin(16, 2);
}

void loop() {
  int xVal = analogRead(x);     //Read inputs
  int yVal = analogRead(y);

  switch(xVal){
   case 600 ... 1024:            //baseR
     if(stepsB < 100){
      shiftRight(m1);
      stepsB +=1;        
     }
    break;
   case 0 ... 400:               //baseL
     if(stepsB > -100){
      shiftLeft(m1);
      stepsB -=1;        
     }
    break;
  }
  
  switch(yVal){
   case 600 ... 1024:             //elbowR
      if(stepsS > -100){
       shiftRight(m2);
       stepsS -=1;
      }
    break;
   case 0 ... 400:                 //elbowL
     if(stepsS < 100){
      shiftLeft(m2);
      stepsS +=1;
     }  
    break;
  }

  writePosB();  //writes the outputs to the H-bridge
  writePosS();  //functions are to keep the main loop small and easy to read
                //also easily allows more motors to be added (eventually)
  //update lcd
  lcd.clear();
  lcd.setCursor(0,0); //set cursor to col1, line 1
  lcd.print(stepsB);
  lcd.setCursor(0,1); //go down 1 line
  lcd.print(stepsS);
  delay(100);
}

int shiftLeft(int j[]){
    int tmp = j[0];       //grabs the first number in the array    
    for (int i=0; i<3; i++){  //0-3 covers each array index
      j[i]=j[i+1];  //sets each array bit to the value of the next bit
    }               //which means everything is shifted over
    j[3] = tmp;   //places the origional first number last
    return j[4];  //passes new array values back into origional array
}

int shiftRight(int j[]){
    int tmp = j[3];           //works the same as the previous function
    for (int i=3; i>0; i--){  //just the opposite way around
      j[i]=j[i-1];
    }
    j[0] = tmp;
    return j[4];
}

void writePosB(){
  digitalWrite(6, m1[0]);   //pin numbers depend on wiring
  digitalWrite(7, m1[1]);   
  digitalWrite(8, m1[2]);   
  digitalWrite(9, m1[3]);
}

void writePosS(){
  digitalWrite(10, m2[0]);
  digitalWrite(11, m2[1]);
  digitalWrite(12, m2[2]);
  digitalWrite(13, m2[3]);
}
