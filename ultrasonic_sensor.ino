const int trig = 9;
const int echo = 8;
const int LED = 3;
long del;

void setup() {
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);  //from HC-SR04 datasheet
  digitalWrite(trig, LOW);

  del = pulseIn(echo, HIGH);
  Serial.println(del);
  if(del < 300){   //adjust del to change distance
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
  delay(60);  //recommended on datasheet
}
