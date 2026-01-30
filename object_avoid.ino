/* =====================================================
   OBJECT AVOIDING ROBOT (MEDIAN FILTERED)
   Arduino UNO + L298N
   2 × HC-SR04 (25° outward, front)
   ===================================================== */

// ---------- L298N PINS ----------
#define ENA 3    // Left motor speed (PWM)
#define IN1 5
#define IN2 6

#define ENB 10   // Right motor speed (PWM)
#define IN3 7
#define IN4 8

// ---------- ULTRASONIC PINS ----------
#define TRIG_L 9
#define ECHO_L 10

#define TRIG_R 11
#define ECHO_R 12

// ---------- SETTINGS ----------
#define SPEED     150
#define WALL_DIST 20    // cm
#define US_SAMPLES 5    // median filter samples

int distL, distR;

// =====================================================
// MOTOR FUNCTIONS
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
// ULTRASONIC FUNCTIONS
// =====================================================
void initUltrasonic() {
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);

  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
}

int readUltrasonicRaw(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;   // no echo

  return duration * 0.034 / 2;
}

// ---------- MEDIAN FILTER ----------
int getMedian(int *arr, int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (arr[j] < arr[i]) {
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
      }
    }
  }
  return arr[n / 2];
}

int readUltrasonicMedian(int trigPin, int echoPin) {
  int samples[US_SAMPLES];

  for (int i = 0; i < US_SAMPLES; i++) {
    samples[i] = readUltrasonicRaw(trigPin, echoPin);
    delay(5);
  }

  return getMedian(samples, US_SAMPLES);
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(9600);
  initMotors();
  initUltrasonic();
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  distL = readUltrasonicMedian(TRIG_L, ECHO_L);
  distR = readUltrasonicMedian(TRIG_R, ECHO_R);

  Serial.print("US_L: ");
  Serial.print(distL);
  Serial.print("  US_R: ");
  Serial.println(distR);

  // BOTH SIDES BLOCKED
  if (distL < WALL_DIST && distR < WALL_DIST) {
    stopMotors();
    delay(100);
    turnRight();
    delay(300);
  }
  // LEFT BLOCKED
  else if (distL < WALL_DIST) {
    turnRight();
  }
  // RIGHT BLOCKED
  else if (distR < WALL_DIST) {
    turnLeft();
  }
  // CLEAR PATH
  else {
    forward();
  }

  delay(50); // stability delay
}
