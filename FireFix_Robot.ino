#include "pins.h"
#include "sensors.h"
#include "motors.h"
#include "fan.h"
#include "logic.h"

void setup() {
  initPins();
  initMotors();
  initFan();
  Serial.begin(9600);
}

void loop() {
  readSensors();
  decisionLoop();
}

