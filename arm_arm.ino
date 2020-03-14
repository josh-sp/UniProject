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
byte stepsE = 100;    //steps is current position (between 0-200)

byte posB = 100;  //pos is desired position (in steps 0-200)
byte posS = 100;  //100 is the default
byte posE = 100;
byte gripper = 0;  //0 is open, 1 is closed

bool move1 = false;    //collision detect variables, false start for safety
bool move2 = false;    //one for left side and one for right
const int trig = 50;   //trigger for HC-SR04 (both tied together)
const int echo1 = 51;  //echo signal for HC-SR04
const int echo2 = 53;
const int led1 = 48;   //warning LED
const int led2 = 49;
unsigned long del1;    //delay time for sensor
unsigned long del2;
int wait = 10;         //switching delay for motor drivers

void setup() { 
  //motor1(base)      motor2(shoulder)    motor3(elbow)     
  pinMode(2, OUTPUT); pinMode(6, OUTPUT); pinMode(10, OUTPUT);
  pinMode(3, OUTPUT); pinMode(7, OUTPUT); pinMode(11, OUTPUT);
  pinMode(4, OUTPUT); pinMode(8, OUTPUT); pinMode(12, OUTPUT);
  pinMode(5, OUTPUT); pinMode(9, OUTPUT); pinMode(13, OUTPUT);

  pinMode(trig, OUTPUT); //only one trigger as theyre tied together
  pinMode(echo1, INPUT); //echo recieves the trigger signal when it bounces back
  pinMode(echo2, INPUT);
  pinMode(led1, OUTPUT); //turns on if collision is detected
  pinMode(led2, OUTPUT);

  grip.attach(46);       //connect the gripper (servo)
  Serial.begin(9600);    //UART matching baud rate from controller
}

void loop() {
  //Read incoming data
  while(Serial.available()<4){}  //waits until 4 serial bytes are recieved (since 4 are sent from the controller)
  for(int i=0; i<4; i++){        //loops 4 times to populate dataIn[] with recieved serial data
    if(Serial.available()>0){    //make sure the serial buffer has data ready
      dataIn[i] = Serial.read(); //load data into array
    }
  } 
  posB = dataIn[0];  //store the desired positions. received from the controller
  posS = dataIn[1];
  posE = dataIn[2];
  gripper = dataIn[3]; //store desired gripper position

  canMove();  //function which checks if there will be a collision 
  if((move1 + move2) == 2){ //true bool = 1, this checks both are true
    go();  //move the motors to the desired position
  }
}

void go(){
  //BASE
  if(stepsB > posB){ //compare current pos to desired pos (too high in this case)
    while(stepsB > posB){ //loop until the desired pos = current pos
      if(stepNB==0){stepNB=3;} //if end of sequence is reached start again
      else{stepNB--;} //else next step in sequence 
      digitalWrite(2, motor[stepNB][0]); //write pin value from array [y][x]
      digitalWrite(3, motor[stepNB][1]);
      digitalWrite(4, motor[stepNB][2]); 
      digitalWrite(5, motor[stepNB][3]);
      stepsB--;    //stepping down so current pos is now 1 less
      delay(wait); //driver switching delay
    }
  }
  if(stepsB < posB){ //loop works in the same way as the previous one
    while(stepsB < posB){ //now checking if value is too low
      if(stepNB==3){stepNB=0;} //if end of sequence is reached start again
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
  if(stepsS > posS){ //same code as the base but with varaibles for the shoulder
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
  if(stepsE > posE){   //same as previous two
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
  if(gripper > 0){grip.write(180);} else{grip.write(0);} //if gripper is true close the grip else open it
}

void canMove(){
  digitalWrite(trig, HIGH); //start pulse
  delayMicroseconds(10);  //from HC-SR04 datasheet
  digitalWrite(trig, LOW); //end pulse (trig has now sent a 10uS pulse)
  del1 = pulseIn(echo1, HIGH); //measure time taken for signal to return
    if(del1 < 400){ //delay of 400 gives around 4-5cm distance
      digitalWrite(led1, HIGH); //turn led on if object is too close
      move1 = false; //arm is not allowed to move
    }
    else{ //if delay is longer than 400 (no object close)
      digitalWrite(led1, LOW); //keep LED off
      move1 = true; //arm is allowed to move if other move variable is true too
    }
  delay(60); //60mS delay between pulses is recommended in datasheet

  digitalWrite(trig, HIGH); //generate 10uS pulse again
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  del2 = pulseIn(echo2, HIGH); //measure delay
    if(del2 < 400){  //same distance as other sensor
      digitalWrite(led2, HIGH);
      move2 = false;
    }
    else{
      digitalWrite(led2, LOW);
      move2 = true;
    }
  delay(60);
}
