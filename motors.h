#ifndef MOTORS_H
#define MOTORS_H

// ---------- SPEED SETTINGS ----------
#define SPEED_FAST 180
#define SPEED_SLOW 120

// ---------- INITIALIZE MOTORS ----------
void initMotors() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);   // VERY IMPORTANT: enable TB6612FNG

#if DEBUG
  Serial.println("MOTOR INIT: TB6612FNG ENABLED");
#endif
}

// ---------- STOP ----------
void stopMotors() {
#if DEBUG
  Serial.println("MOTOR: STOP");
#endif
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

// ---------- FORWARD ----------
void forward(int speed = SPEED_FAST) {
#if DEBUG
  Serial.print("MOTOR: FORWARD | SPEED = ");
  Serial.println(speed);
#endif

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
}

// ---------- FORWARD SLOW ----------
void forwardSlow() {
#if DEBUG
  Serial.println("MOTOR: FORWARD SLOW");
#endif
  forward(SPEED_SLOW);
}

// ---------- TURN LEFT ----------
void turnLeft() {
#if DEBUG
  Serial.println("MOTOR: TURN LEFT");
#endif

  // Left motor backward, right motor forward
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, SPEED_SLOW);
  analogWrite(PWMB, SPEED_SLOW);
}

// ---------- TURN RIGHT ----------
void turnRight() {
#if DEBUG
  Serial.println("MOTOR: TURN RIGHT");
#endif

  // Left motor forward, right motor backward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  analogWrite(PWMA, SPEED_SLOW);
  analogWrite(PWMB, SPEED_SLOW);
}

// ---------- BACK OFF ----------
void backOff() {
#if DEBUG
  Serial.println("MOTOR: BACK OFF");
#endif

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  analogWrite(PWMA, SPEED_SLOW);
  analogWrite(PWMB, SPEED_SLOW);

  delay(300);     // move backward briefly
  stopMotors();
}

#endif
