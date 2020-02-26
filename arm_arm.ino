/* stepper sequence is as follows:
 *  step   C0 C1 C2 C3
 *     0    1  0  1  0
 *     1    0  1  1  0
 *     2    0  1  0  1
 *     3    1  0  0  1
 */ //this is stored in the array below:

#include <Servo.h> //only used for gripper
Servo grip;
 
int motor[4][4]={
  {1, 0, 1, 0},
  {0, 1, 1, 0},
  {0, 1, 0, 1},
  {1, 0, 0, 1}
 };
 
byte dataIn[4];     //base, shoulder, elbow, gripper
int stepNB = 0;     //keeps track of step in sequence
int stepNS = 0;     //sequence is 4 steps long (at top)
int stepNE = 0;

byte stepsB = 100;    //bytes are used for easier comparison of values
byte stepsS = 100;    //no conversion between datatypes int->byte
byte stepsE = 100;
byte posB = 100;  //desired position of motor (in steps 0-200)
byte posS = 100;  //100 is the default
byte posE = 100;
byte gripper = 0;

bool move1 = false;    //collision detect variables, false start for safety
bool move2 = false;
const int trig = 50;
const int echo1 = 51;
const int echo2 = 53;
const int led1 = 48;
const int led2 = 49;
unsigned long del1;
unsigned long del2;
int wait = 10;

void setup() { 
  //motor1(base)      motor2(shoulder)    motor3(elbow)     
  pinMode(2, OUTPUT); pinMode(6, OUTPUT); pinMode(10, OUTPUT);
  pinMode(3, OUTPUT); pinMode(7, OUTPUT); pinMode(11, OUTPUT);
  pinMode(4, OUTPUT); pinMode(8, OUTPUT); pinMode(12, OUTPUT);
  pinMode(5, OUTPUT); pinMode(9, OUTPUT); pinMode(13, OUTPUT);

  pinMode(trig, OUTPUT); //only one trigger as theyre tied together
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  grip.attach(46);
  Serial.begin(9600);
}

void loop() {
  //Read incoming data
  while(Serial.available()<4){}
  for(int i=0; i<4; i++){
    if(Serial.available()>0){
      dataIn[i] = Serial.read();
    }
  } 
  posB = dataIn[0];
  posS = dataIn[1];
  posE = dataIn[2];
  gripper = dataIn[3]; 

  canMove();
  if((move1 + move2) == 2){ //true bool = 1, this checks both are true
    go();
  }
}

void go(){
  //BASE
  if(stepsB > posB){ //checks if position is correct
    while(stepsB > posB){
      if(stepNB==0){stepNB=3;}
      else{stepNB--;}
      digitalWrite(2, motor[stepNB][0]); //reads the array values[y][x]
      digitalWrite(3, motor[stepNB][1]);
      digitalWrite(4, motor[stepNB][2]); 
      digitalWrite(5, motor[stepNB][3]);
      stepsB--;
      delay(wait);
    }
  }
  if(stepsB < posB){
    while(stepsB < posB){
      if(stepNB==3){stepNB=0;}
      else{stepNB++;}
      digitalWrite(2, motor[stepNB][0]);
      digitalWrite(3, motor[stepNB][1]);
      digitalWrite(4, motor[stepNB][2]); 
      digitalWrite(5, motor[stepNB][3]);
      stepsB++;
      delay(wait);
    }
  } 

  //SHOULDER
  if(stepsS > posS){
    while(stepsS > posS){
      if(stepNS==0){stepNS=3;}
      else{stepNS--;}
      digitalWrite(6, motor[stepNS][0]);
      digitalWrite(7, motor[stepNS][1]);
      digitalWrite(8, motor[stepNS][2]); 
      digitalWrite(9, motor[stepNS][3]);
      stepsS--;
      delay(wait);
    }
  }
  if(stepsS< posS){
    while(stepsS < posS){
      if(stepNS==3){stepNS=0;}
      else{stepNS++;}
      digitalWrite(6, motor[stepNS][0]);
      digitalWrite(7, motor[stepNS][1]);
      digitalWrite(8, motor[stepNS][2]); 
      digitalWrite(9, motor[stepNS][3]);
      stepsS++;
      delay(wait);
    }
  }

  //ELBOW
  if(stepsE > posE){
    while(stepsE > posE){
      if(stepNE==0){stepNE=3;}
      else{stepNE--;}
      digitalWrite(10, motor[stepNE][0]); 
      digitalWrite(11, motor[stepNE][1]);
      digitalWrite(12, motor[stepNE][2]); 
      digitalWrite(13, motor[stepNE][3]);
      stepsE--;
      delay(wait);
    }
  }
  if(stepsE < posE){
    while(stepsE < posE){
      if(stepNE==3){stepNE=0;}
      else{stepNE++;}
      digitalWrite(10, motor[stepNE][0]);
      digitalWrite(11, motor[stepNE][1]);
      digitalWrite(12, motor[stepNE][2]); 
      digitalWrite(13, motor[stepNE][3]);
      stepsE++;
      delay(wait);
    }
  }
  if(gripper > 0){grip.write(180);} else{grip.write(0);}
}

void canMove(){
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);  //from HC-SR04 datasheet
  digitalWrite(trig, LOW);
  del1 = pulseIn(echo1, HIGH);
    if(del1 < 400){
      digitalWrite(led1, HIGH);
      move1 = false;
    }
    else{
      digitalWrite(led1, LOW);
      move1 = true;
    }
  delay(60);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  del2 = pulseIn(echo2, HIGH);
    if(del2 < 400){
      digitalWrite(led2, HIGH);
      move2 = false;
    }
    else{
      digitalWrite(led2, LOW);
      move2 = true;
    }
  delay(60);
}
