int motor[4][4] = { 
  {0, 1, 1, 0},     //stores each step in an array row
  {0, 1, 0, 1},     //y val is stored and the corresponding...
  {1, 0, 0, 1},     //row is then written to the pins
  {1, 0, 1, 0}
};    
int col1, col2, col3 = 0; //stores positional data
byte col[3];  //reads incoming data about how to move
int stepsB, stepsS, stepsE = 0;

const int trig1 = 50;   //collision detection pins
const int echo1 = 51;   //one for left side, one for right
const int trig2 = 52;
const int echo2 = 53;
long del1;  //will store the response times of sensors
long del2;
bool Move = true;

void setup() {
  //motor1(base)      motor2(shoulder)    motor3(elbow)     
  pinMode(2, OUTPUT); pinMode(6, OUTPUT); pinMode(10, OUTPUT);
  pinMode(3, OUTPUT); pinMode(7, OUTPUT); pinMode(11, OUTPUT);
  pinMode(4, OUTPUT); pinMode(8, OUTPUT); pinMode(12, OUTPUT);
  pinMode(5, OUTPUT); pinMode(9, OUTPUT); pinMode(13, OUTPUT);

  pinMode(50, OUTPUT); pinMode(51, INPUT);  //collision sensors
  pinMode(52, OUTPUT); pinMode(53, INPUT);
  
  Serial.begin(9600); //matching baud rate
}

void loop() {
  //read the data
  while(Serial.available()<3){} //wait for all 3 bytes
  for(int i=0; i<3; i++){
    if(Serial.available()>0){
      col[i] = Serial.read();  
    }
  }

//  canMove();
//  while(Move == false){canMove;} //keep checking for move condition
  
  //parse the user input
  switch(col[0]){ //base
    case 1:
     if(stepsB < 100){
      if(col1 >= 3){col1=0;}
      else{col1 +=1;}
      stepsB +=1;      
     }
     break;
    case 2:
     if(stepsB > -100){
      if(col1==0){col1=3;}
      else{col1 -=1;}      
      stepsB -=1;        
     }
     break;
  }
  switch(col[1]){ //shoulder
    case 1:
     if(stepsS < 100){
      if(col2 >= 3){col2=0;}
      else{col2 +=1;}
      stepsS +=1;      
     }
     break;
    case 2:
     if(stepsS > -100){
      if(col2==0){col2=3;}
      else{col2 -=1;}      
      stepsS -=1;        
     }
     break;
  }
  switch(col[2]){ //shoulder
    case 1:
     if(stepsE < 100){
      if(col3 >= 3){col3=0;}
      else{col3 +=1;}
      stepsE +=1;      
     }
     break;
    case 2:
     if(stepsE > -100){
      if(col3==0){col3=3;}
      else{col3 -=1;}      
      stepsE -=1;        
     }
     break;
  }

  //write motor outputs
  digitalWrite(2, motor[col1][0]);   //pin numbers depend on wiring
  digitalWrite(3, motor[col1][1]);   //writes a row to the output
  digitalWrite(4, motor[col1][2]);   //yInd1 stores the column val
  digitalWrite(5, motor[col1][3]);

  digitalWrite(6, motor[col2][0]);
  digitalWrite(7, motor[col2][1]);
  digitalWrite(8, motor[col2][2]);
  digitalWrite(9, motor[col2][3]);

  digitalWrite(10, motor[col3][0]);
  digitalWrite(11, motor[col3][1]);
  digitalWrite(12, motor[col3][2]);
  digitalWrite(13, motor[col3][3]);

  delay(50); //stops motor drivers from switching too fast
}

void canMove(){
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);      //value from datasheet
  digitalWrite(trig1, LOW);
  del1 = pulseIn(echo1, HIGH);

  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  del2 = pulseIn(echo2, HIGH);

  if(del1 > 300){
    Move = false;
  }
  else if(del2 > 300){
    Move = false;
  }
  else{
    Move = true;
  }
}
