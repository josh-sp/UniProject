const int trig = 9;
const int echo = 8;
long del;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);  //from HC-SR04 datasheet
  digitalWrite(trig, LOW);

  del = pulseIn(echo, HIGH);
  Serial.println(del);
  delay(60);  //recommended on datasheet
}
