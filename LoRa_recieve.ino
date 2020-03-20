void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);     //LED indicator
}

void loop() {
  if(Serial.read() == 1){
    digitalWrite(2, HIGH);
  }                          
  if(Serial.read() == 0){   
    digitalWrite(2, LOW);
  }
}
