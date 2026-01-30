/* =====================================================
   OBSTACLE AVOIDING ROBOT
   Arduino UNO + TB6612FNG
   2 × HC-SR04 (25° outward)
   ===================================================== */

// ---------- TB6612FNG PINS ----------
#define AIN1 5
#define AIN2 6
#define PWMA 3

#define BIN1 7
#define BIN2 8
#define PWMB 10

#define STBY 4

// ---------- ULTRASONIC PINS ----------
#define TRIG_L 9
#define ECHO_L 10

#define TRIG_R 11
#define ECHO_R 12

// ---------- SETTINGS ----------
#define SPEED 150
#define WALL_DIST 20   // cm

int distL, distR;

// ---------- MOTOR FUNCTIONS ----------
void initMotors() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH); // enable TB6612FNG
}

void forward() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, SPEED);
  analogWrite(PWMB, SPEED);
}

void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void turnLeft() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, SPEED);
  analogWrite(PWMB, SPEED);
}

void turnRight() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMA, SPEED);
  analogWrite(PWMB, SPEED);
}

// ---------- ULTRASONIC FUNCTIONS ----------
void initUltrasonic() {
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);

  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
}

int readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

void readUltrasonics() {
  distL = readUltrasonic(TRIG_L, ECHO_L);
  distR = readUltrasonic(TRIG_R, ECHO_R);
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(9600);
  initMotors();
  initUltrasonic();
}

// ---------- LOOP ----------
void loop() {
  readUltrasonics();

  Serial.print("US_L: ");
  Serial.print(distL);
  Serial.print("  US_R: ");
  Serial.println(distR);

  // BOTH SIDES BLOCKED → TURN
  if (distL < WALL_DIST && distR < WALL_DIST) {
    stopMotors();
    delay(100);
    turnRight();
    delay(300);
  }
  // LEFT WALL → TURN RIGHT
  else if (distL < WALL_DIST) {
    turnRight();
  }
  // RIGHT WALL → TURN LEFT
  else if (distR < WALL_DIST) {
    turnLeft();
  }
  // CLEAR PATH
  else {
    forward();
  }

  delay(50); // stability delay
}
