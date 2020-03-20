void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);     //LED indicator
}

void loop() {
  if(Serial.read() == 1){
    digitalWrite(2, HIGH);  //default state is high anyway
  }                         //because of pullup resistor 
  if(Serial.read() == 0){   //at the input stage
    digitalWrite(2, LOW);
  }
}
