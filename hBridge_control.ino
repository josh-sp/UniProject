const int x = A0;
const int y = A1;
String xDir;
String yDir;

const int b1 = 2; //o for coil one
const int a1 = 3;
const int b2 = 4; //t for coil two
const int a2 = 5;
int m1[4] = {1, 0, 0, 0};
int steps = 0;

void setup() {
  pinMode(b1, OUTPUT);
  pinMode(a1, OUTPUT);    
  pinMode(b2, OUTPUT);
  pinMode(a2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int xVal = analogRead(x);     //Read inputs
  int yVal = analogRead(y);

  switch(xVal){
    case 600 ... 1024:
      xDir = "Right";
      turnRight(m1);
      break;
    case 0 ... 400:
      xDir = "Left";
      turnLeft(m1);
      break;
    default:
      xDir = "Center";
      break;
  }

  for(int i=0; i<3; i++){     //motor is 4 pins (2, 3, 4, 5)
    int n = 2;                //pins start at 2
    digitalWrite(n, m1[i]);   //array holds coil values for each pin
    n +=1;                    //next pin value is written
  }                           
  Serial.println(steps);
  delay(100);
}

void turnLeft(int j[]){
   if(steps < 100){
      int tmp = j[0];           
      for (int i=0; i<3; i++){  //3 is the array length -1
        j[i]=j[i+1];
      }
      j[3] = tmp;
      steps +=1;
      return j[4];
  }
}

void turnRight(int j[]){
   if(steps > -100){
      int tmp = j[3];           
      for (int i=3; i>0; i--){  //3 is the array length -1
        j[i]=j[i-1];
      }
      j[0] = tmp;
      steps +=1;
      return j[4];
  }
}
