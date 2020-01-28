int m1[4][4] = {    //m1= coil1: 6&7, coil2: 8&9
  {0, 1, 1, 0},     //stores each step in an array row
  {0, 1, 0, 1},     //y val is stored and the corresponding...
  {1, 0, 0, 1},     //row is then written to the pins
  {1, 0, 1, 0}
};   
int m2[4][4] = {    //m2= coil1: 10&11, coil2: 12&13
  {0, 1, 1, 0},
  {0, 1, 0, 1},
  {1, 0, 0, 1},
  {1, 0, 1, 0}
};   

int col1, col2, col3;

void setup() {
  //motor1(base)      motor2(shoulder)    motor3(elbow)     
  pinMode(2, OUTPUT); pinMode(6, OUTPUT); pinMode(10, OUTPUT);
  pinMode(3, OUTPUT); pinMode(7, OUTPUT); pinMode(11, OUTPUT);
  pinMode(4, OUTPUT); pinMode(8, OUTPUT); pinMode(12, OUTPUT);
  pinMode(5, OUTPUT); pinMode(9, OUTPUT); pinMode(13, OUTPUT);
  
  Serial.begin(9600); //matching baud rate
}

void loop() {
  while(Serial.available()){  //waits for serial port
    col1 = Serial.read();
    col1 -=1; //values are +1 what they should be so 0 is never sent
  }  
  writePosB();
}

void writePosB(){
  digitalWrite(2, m1[col1][0]);   //pin numbers depend on wiring
  digitalWrite(3, m1[col1][1]);   //writes a row to the output
  digitalWrite(4, m1[col1][2]);   //yInd1 stores the column val
  digitalWrite(5, m1[col1][3]);
}
