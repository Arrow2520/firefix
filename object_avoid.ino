/* =====================================================
   FIRE FIGHTING ROBOT
   Object Avoidance + Flame Approach + Relay Fan
   Arduino UNO + L298N
   ===================================================== */

// ================== L298N ==================
#define ENA 3
#define IN1 5
#define IN2 6

#define ENB 10
#define IN3 7
#define IN4 8

// ================== ULTRASONIC ==================
#define TRIG_L 9
#define ECHO_L 2      // FIXED (no conflict)

#define TRIG_R 11
#define ECHO_R 12

// ================== FLAME SENSORS ==================
#define F1 A0
#define F2 A1
#define F3 A2   // center
#define F4 A3
#define F5 A4

// ================== FAN (RELAY) ==================
#define FAN_PIN 13    // relay input

// ================== TUNING ==================
#define SPEED            180
#define WALL_DIST        40    // cm
#define FLAME_NEAR_DIST  20    // cm
#define FLAME_TH         600
#define US_SAMPLES       5

int distL, distR;
int f1, f2, f3, f4, f5;

// =====================================================
// MOTOR CONTROL (L298N)
// =====================================================
void initMotors() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

// =====================================================
// ULTRASONIC (MEDIAN FILTER)
// =====================================================
void initUltrasonic() {
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
}

int readUltrasonicRaw(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long d = pulseIn(echo, HIGH, 30000);
  if (d == 0) return 999;
  return d * 0.034 / 2;
}

int median(int *a, int n) {
  for (int i = 0; i < n - 1; i++)
    for (int j = i + 1; j < n; j++)
      if (a[j] < a[i]) {
        int t = a[i]; a[i] = a[j]; a[j] = t;
      }
  return a[n / 2];
}

int readUltrasonicMedian(int trig, int echo) {
  int s[US_SAMPLES];
  for (int i = 0; i < US_SAMPLES; i++) {
    s[i] = readUltrasonicRaw(trig, echo);
    delay(5);
  }
  return median(s, US_SAMPLES);
}

// =====================================================
// FLAME
// =====================================================
void readFlame() {
  f1 = analogRead(F1);
  f2 = analogRead(F2);
  f3 = analogRead(F3);
  f4 = analogRead(F4);
  f5 = analogRead(F5);
}

bool flameDetected() {
  return (f1 < FLAME_TH || f2 < FLAME_TH || f3 < FLAME_TH ||
          f4 < FLAME_TH || f5 < FLAME_TH);
}

void goToFlame() {
  if (f3 < f1 && f3 < f5) {
    forward();
  }
  else if (f1 < f5) {
    turnLeft();
  }
  else {
    turnRight();
  }
}

// =====================================================
// FAN (RELAY)
// =====================================================
// If your relay is ACTIVE-LOW, swap HIGH/LOW here
void fanOn()  { digitalWrite(FAN_PIN, HIGH); }
void fanOff() { digitalWrite(FAN_PIN, LOW);  }

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(9600);

  initMotors();
  initUltrasonic();

  pinMode(FAN_PIN, OUTPUT);
  fanOff();

  Serial.println("FIRE FIGHTING ROBOT READY");
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  distL = readUltrasonicMedian(TRIG_L, ECHO_L);
  distR = readUltrasonicMedian(TRIG_R, ECHO_R);
  readFlame();

  // ================= MAIN LOGIC =================
  if (flameDetected()) {

    // 🔥 VERY CLOSE → STOP + FAN
    if (distL < FLAME_NEAR_DIST || distR < FLAME_NEAR_DIST) {
      stopMotors();
      fanOn();
      delay(2000);
      fanOff();
    }

    // 🧱 WALL SAFETY WHILE APPROACHING FLAME
    else if (distL < WALL_DIST && distR < WALL_DIST) {
      stopMotors();
      delay(60);
      turnRight();
      delay(200);
    }
    else if (distL < WALL_DIST) {
      turnRight();
    }
    else if (distR < WALL_DIST) {
      turnLeft();
    }

    // 🔥 MOVE TOWARD FLAME
    else {
      fanOff();
      goToFlame();
    }
  }

  // 🚧 NO FLAME → OBSTACLE AVOIDANCE
  else {
    fanOff();

    if (distL < WALL_DIST && distR < WALL_DIST) {
      stopMotors();
      delay(60);
      turnRight();
      delay(600);
    }
    else if (distL < WALL_DIST) {
      turnRight();
    }
    else if (distR < WALL_DIST) {
      turnLeft();
    }
    else {
      forward();
    }
  }

  delay(40);
}
