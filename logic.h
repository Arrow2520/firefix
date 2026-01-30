#define TH1 600
#define TH2 600
#define TH3 600
#define TH4 600
#define TH5 600

enum State {
  STATE_LINE,
  STATE_FLAME,
  STATE_WALL,
  STATE_SEARCH
};

State currentState;

bool lineDetected() {
  return (lineL == HIGH || lineR == HIGH);
}

bool flameDetected() {
  return (f1 < TH1 || f2 < TH2 || f3 < TH3 || f4 < TH4 || f5 < TH5);
}

bool wallClose() {
  return (distance < 20);
}

void extinguish() {
  stopMotors();
  fanON();
  delay(2000);
  fanOFF();
  backOff();
}

void goToFlame() {
  int left = f1 + f2;
  int right = f4 + f5;

  if (f3 < left && f3 < right) forward();
  else if (left < right) turnLeft();
  else turnRight();
}

void decisionLoop() {

  if (lineDetected()) {
#if DEBUG
    Serial.println("STATE: LINE → EXTINGUISH");
#endif
    currentState = STATE_LINE;
    extinguish();
  }

  else if (flameDetected()) {
#if DEBUG
    Serial.println("STATE: FLAME → ALIGN");
#endif
    currentState = STATE_FLAME;

    if (distance < 12) {
#if DEBUG
      Serial.println("FLAME MODE: TOO CLOSE → STOP");
#endif
      stopMotors();
    } else {
      goToFlame();
    }
  }

  else if (wallClose()) {
#if DEBUG
    Serial.println("STATE: WALL → AVOID");
#endif
    currentState = STATE_WALL;
    turnRight();
    delay(300);
  }

  else {
#if DEBUG
    Serial.println("STATE: SEARCH");
#endif
    currentState = STATE_SEARCH;
    forward();
  }
}

