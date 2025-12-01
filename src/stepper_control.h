#pragma once
#include <Arduino.h>

void moveStepperBackAndForth();
void moveInCircle(float radius_mm, int steps_per_revolution, int speed_rpm);
