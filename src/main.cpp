#include <Arduino.h>
#include "stepper_control.h"

void setup() {
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  delay(5000);
  //moveStepperBackAndForthLerp();
  moveXAxis(400);
  //moveInCircle(50.0, 800, 30); // 50mm radius, 200 steps/rev, 30 RPM
  
}