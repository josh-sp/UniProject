void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); //button input
}

void loop() {
  if(digitalRead(2) == HIGH){
    Serial.write(0);
  }
  else{
    Serial.write(1);
  }
}
