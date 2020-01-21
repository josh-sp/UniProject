int data;
int prev;

void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  data = Serial.read();
  
  switch(data){
    case 10:
     digitalWrite(8, HIGH);
     prev = 8;
    break;
    case 20:
     digitalWrite(9, HIGH);
     prev = 9;
    break;
    case 30:
     digitalWrite(10, HIGH);
     prev = 10;
    break;
    case 40:
     digitalWrite(11, HIGH);
     prev = 11;
    break;
    default:
     digitalWrite(prev, HIGH);
    break;
  }
  delay(50);
}
