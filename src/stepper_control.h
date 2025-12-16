#pragma once
#include <Arduino.h>

void moveStepperBackAndForth();
void moveStepperBackAndForthLerp();
void moveInCircle(float radius_mm, int steps_per_revolution, int speed_rpm);
void moveWithLerp(float distance_mm, float pulley_circumference, int steps_per_rev, int pulse_pin, int dir_pin, int ramp_pulses, long min_delay, long max_delay);
void moveXAxis(float distance_mm);

