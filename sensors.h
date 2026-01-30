#ifndef SENSORS_H
#define SENSORS_H

// --------- GLOBAL SENSOR VARIABLES ----------
int f1, f2, f3, f4, f5;
int lineL, lineR;
int distL, distR;

// --------- INIT PINS ----------
void initPins() {
  pinMode(LINE_L, INPUT);
  pinMode(LINE_R, INPUT);

  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);

  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
}

// --------- FLAME SENSORS ----------
void readFlameSensors() {
  f1 = analogRead(F1);
  f2 = analogRead(F2);
  f3 = analogRead(F3);
  f4 = analogRead(F4);
  f5 = analogRead(F5);
}

// --------- LINE SENSORS ----------
void readLineSensors() {
  lineL = digitalRead(LINE_L);
  lineR = digitalRead(LINE_R);
}

// --------- GENERIC ULTRASONIC READ ----------
int readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long d = pulseIn(echoPin, HIGH, 30000);
  if (d == 0) return 999;

  return d * 0.034 / 2;
}

// --------- READ BOTH ULTRASONICS ----------
void readUltrasonics() {
  distL = readUltrasonic(TRIG_L, ECHO_L);
  distR = readUltrasonic(TRIG_R, ECHO_R);
}

// --------- MASTER READ ----------
void readSensors() {
  readFlameSensors();
  readLineSensors();
  readUltrasonics();

#if DEBUG
  Serial.print("FLAME: ");
  Serial.print(f1); Serial.print(" ");
  Serial.print(f2); Serial.print(" ");
  Serial.print(f3); Serial.print(" ");
  Serial.print(f4); Serial.print(" ");
  Serial.print(f5);

  Serial.print(" | LINE: ");
  Serial.print(lineL);
  Serial.print(" ");
  Serial.print(lineR);

  Serial.print(" | US L: ");
  Serial.print(distL);
  Serial.print("  US R: ");
  Serial.println(distR);
#endif
}

#endif


