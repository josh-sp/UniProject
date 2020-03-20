void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT); //button input
}

void loop() {
  if(digitalRead(2) == HIGH){
    Serial.write(1);
  }
  else{
    Serial.write(0);
  }
}
