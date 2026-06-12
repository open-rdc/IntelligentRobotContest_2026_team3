int syuuki = 20000;

void setup() {
  pinMode(13, OUTPUT);
  //Serial.begin(9600);
}

void loop() {
  int sV = analogRead(A0);
  Serial.println(sV);
  long high = (long)sV * syuuki / 1023;
  long low  = syuuki - high;

  if (high <= 0){
    digitalWrite(13, LOW);
  }
  else if (high >= syuuki){
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13, HIGH);
    delayMicroseconds(high);

    digitalWrite(13, LOW);
    delayMicroseconds(low);
  }
}
