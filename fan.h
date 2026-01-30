void initFan() {
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
}

void fanON() {
  
#if DEBUG
  Serial.println("FAN: ON");
#endif

  digitalWrite(FAN_PIN, HIGH);
}

void fanOFF() {

#if DEBUG
  Serial.println("FAN: OFF");
#endif

  digitalWrite(FAN_PIN, LOW);
}

