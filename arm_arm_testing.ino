//motor step pattern
int motor[4][4] = { 
  {0, 1, 1, 0},     //stores each step in an array row
  {0, 1, 0, 1},     //y val is stored and the corresponding...
  {1, 0, 0, 1},     //row is then written to the pins
  {1, 0, 1, 0}
};

//variables for position
int col1, col2, col3 = 0; //stores positional data
byte dataIn[3];  //reads incoming data about how to move
int inStepsB, inStepsS, inStepsE = 0;
int stepsB, stepsS, stepsE = 0;

//collision detection
const int trig = 50;
const int echo1 = 51;
const int echo2 = 53;
const int led1 = 48;
const int led2 = 49;
unsigned long del1;
unsigned long del2;
bool Move = true;

void setup() {
  //motor1(base)      motor2(shoulder)    motor3(elbow)     
  pinMode(2, OUTPUT); pinMode(6, OUTPUT); pinMode(10, OUTPUT);
  pinMode(3, OUTPUT); pinMode(7, OUTPUT); pinMode(11, OUTPUT);
  pinMode(4, OUTPUT); pinMode(8, OUTPUT); pinMode(12, OUTPUT);
  pinMode(5, OUTPUT); pinMode(9, OUTPUT); pinMode(13, OUTPUT);

  //collision detection pins
  pinMode(48, OUTPUT); pinMode(49, OUTPUT);
  pinMode(51, INPUT); pinMode(53, INPUT);
  pinMode(50, OUTPUT);
  
  Serial.begin(9600); //matching baud rate to controller
}

void loop() {
  //wait for 3 bytes of data
  while(Serial.available()<3){}
  for(int i=0; i<3; i++){
    if(Serial.available()>0){
      dataIn[i] = Serial.read();
    }
  }
  //convert incoming range (0-200) to a step position (-100, 100)
  //this is because bytes cannot have negative values
  inStepsB = map(dataIn[0], 0, 200, -100, 100);
  inStepsS = map(dataIn[1], 0, 200, -100, 100);
  inStepsE = map(dataIn[2], 0, 200, -100, 100);

//  canMove(); //checks if motors can move or not

  //BASE (check if position is correct, if not move to position)
  if(stepsB > inStepsB){
    while(stepsB > inStepsB){
      if(col1==0){col1=3;}
      else{col1 -=1;}
      digitalWrite(2, motor[col1][0]); //reads the array values from
      digitalWrite(3, motor[col1][1]); //the start of the program
      digitalWrite(4, motor[col1][2]); 
      digitalWrite(5, motor[col1][3]);
      --stepsB;
    }
  }
  if(stepsB < inStepsB){
    while(stepsB < inStepsB){
      if(col1 >= 3){col1=0;}
      else{col1 +=1;}
      digitalWrite(2, motor[col1][0]);
      digitalWrite(3, motor[col1][1]);
      digitalWrite(4, motor[col1][2]);
      digitalWrite(5, motor[col1][3]);
      ++stepsB;
    }
  }
  
  //Shoulder (works same as base)
  if(stepsS > inStepsS){
    while(stepsS > inStepsS){
      if(col2==0){col2=3;}
      else{col2 -=1;}
      digitalWrite(6, motor[col2][0]);   //pin numbers depend on wiring
      digitalWrite(7, motor[col2][1]);   //writes a row to the output
      digitalWrite(8, motor[col2][2]);   //yInd1 stores the column val
      digitalWrite(9, motor[col2][3]);
      --stepsS;
    }
  }
  if(stepsS < inStepsS){
    while(stepsS < inStepsS){
      if(col2 >= 3){col2=0;}
      else{col2 +=1;}
      digitalWrite(6, motor[col2][0]);
      digitalWrite(7, motor[col2][1]);
      digitalWrite(8, motor[col2][2]);
      digitalWrite(9, motor[col2][3]);
      ++stepsS;
    }
  }

  //ELBOW (works same as others)
  if(stepsE > inStepsE){
    while(stepsE > inStepsE){
      if(col3==0){col3=3;}
      else{col3 -=1;}
      digitalWrite(10, motor[col3][0]);   //pin numbers depend on wiring
      digitalWrite(11, motor[col3][1]);   //writes a row to the output
      digitalWrite(12, motor[col3][2]);   //yInd1 stores the column val
      digitalWrite(13, motor[col3][3]);
      --stepsE;
    }
  }
  if(stepsE < inStepsE){
    while(stepsE < inStepsE){
      if(col3 >= 3){col3=0;}
      else{col3 +=1;}
      digitalWrite(10, motor[col3][0]);
      digitalWrite(11, motor[col3][1]);
      digitalWrite(12, motor[col3][2]);
      digitalWrite(13, motor[col3][3]);
      ++stepsE;
    }
  }
  delay(60);
}

void canMove(){
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);  //from HC-SR04 datasheet
  digitalWrite(trig, LOW);    
  del1 = pulseIn(echo1, HIGH);
  if(del1 < 400){   //adjust del to change distance
    digitalWrite(led1, HIGH);
    Move = false;
  }
  else{
    digitalWrite(led1, LOW);
   Move = true;
  }

//  digitalWrite(trig, HIGH);
//  delayMicroseconds(10);  
//  digitalWrite(trig, LOW);   
//  del2 = pulseIn(echo2, HIGH);
//  if(del2 < 400){   //adjust del to change distance
//    digitalWrite(led2, HIGH);
//    Move = false;
//  }
//  else{
//    digitalWrite(led2, LOW);
//    Move = true;
//  }
  
  //function becomes recursive until the motors can move
//  if(Move == false){
//    canMove();
//  }
}
