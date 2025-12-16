#include <Arduino.h>

// Define stepper motor connections
const int DIR_PIN_1 = 9;
const int PULSE_PIN_1 = 8;
const int DIR_PIN_2 = 7;
const int PULSE_PIN_2 = 6;
const int DIR_PIN_3 = 5;
const int PULSE_PIN_3 = 4;
const int PULSE_DELAY_US = 500; // in microseconds

// Function to generate a single pulse on two motors
void pulseMotors() {
    digitalWrite(PULSE_PIN_1, HIGH);
    digitalWrite(PULSE_PIN_2, HIGH);
    delayMicroseconds(PULSE_DELAY_US);
    digitalWrite(PULSE_PIN_1, LOW);
    digitalWrite(PULSE_PIN_2, LOW);
    delayMicroseconds(PULSE_DELAY_US);
}

// Function to move motors with acceleration and deceleration ramping
void moveWithRamping(long total_pulses, int ramp_pulses, int min_delay, int max_delay) {
    // Acceleration phase
    for (int i = 0; i < ramp_pulses; i++) {
        long current_delay = map(i, 0, ramp_pulses - 1, max_delay, min_delay);
        digitalWrite(PULSE_PIN_1, HIGH);
        digitalWrite(PULSE_PIN_2, HIGH);
        delayMicroseconds(current_delay);
        digitalWrite(PULSE_PIN_1, LOW);
        digitalWrite(PULSE_PIN_2, LOW);
        delayMicroseconds(current_delay);
    }

    // Constant speed phase
    for (long i = 0; i < total_pulses - 2 * ramp_pulses; i++) {
        pulseMotors();
    }

    // Deceleration phase
    for (int i = 0; i < ramp_pulses; i++) {
        long current_delay = map(i, 0, ramp_pulses - 1, min_delay, max_delay);
        digitalWrite(PULSE_PIN_1, HIGH);
        digitalWrite(PULSE_PIN_2, HIGH);
        delayMicroseconds(current_delay);
        digitalWrite(PULSE_PIN_1, LOW);
        digitalWrite(PULSE_PIN_2, LOW);
        delayMicroseconds(current_delay);
    }
}

void moveWithLerp(long total_pulses, int ramp_pulses, long min_delay, long max_delay) {

    // -------- Acceleration --------
    for (int i = 0; i < ramp_pulses; i++) {
        float t = (float)i / (float)(ramp_pulses - 1);   // 0 → 1
        long delay_us = lerp(max_delay, min_delay, t);
        pulseMotors(delay_us);
    }

    // -------- Constant speed --------
    long cruise_pulses = total_pulses - 2L * ramp_pulses;
    for (long i = 0; i < cruise_pulses; i++) {
        pulseMotors(min_delay);
    }

    // -------- Deceleration --------
    for (int i = 0; i < ramp_pulses; i++) {
        float t = (float)i / (float)(ramp_pulses - 1);   // 0 → 1
        long delay_us = lerp(min_delay, max_delay, t);
        pulseMotors(delay_us);
    }
}

inline long lerp(long a, long b, float t) {
    return a + (long)((b - a) * t);
}

// Move X and Y axes back and forth
void moveStepperBackAndForth() {
  long num_pulses = 2500;
  int ramp_pulses = 500;
  int min_delay = 450;
  int max_delay = 2000;

  // --- Move axes in one direction ---
  digitalWrite(DIR_PIN_1, HIGH);
  digitalWrite(DIR_PIN_2, HIGH);
  moveWithRamping(num_pulses, ramp_pulses, min_delay, max_delay);
  delay(100);

  // --- Move axes in the opposite direction ---
  digitalWrite(DIR_PIN_1, LOW);
  digitalWrite(DIR_PIN_2, LOW);
  moveWithRamping(num_pulses, ramp_pulses, min_delay, max_delay);
  delay(400);
}

void moveStepperBackAndForthLerp() {
    long total_pulses = 2500;
    int  ramp_pulses  = 500;
    long min_delay    = 450;
    long max_delay    = 2000;

    // Forward
    digitalWrite(DIR_PIN_1, HIGH);
    digitalWrite(DIR_PIN_2, HIGH);
    moveWithLerp(total_pulses, ramp_pulses, min_delay, max_delay);
    delay(200);

    // Backward
    digitalWrite(DIR_PIN_1, LOW);
    digitalWrite(DIR_PIN_2, LOW);
    moveWithLerp(total_pulses, ramp_pulses, min_delay, max_delay);
    delay(500);
}

// Moves the head in a circle on the XY plane
void moveInCircle(float radius_mm, int steps_per_revolution, int speed_rpm) {
  float angle_step = (2 * PI) / steps_per_revolution;
  long step_delay_us = (60L * 1000L * 1000L) / (steps_per_revolution * speed_rpm);

  for (int i = 0; i < steps_per_revolution; i++) {
    float angle = i * angle_step;
    
    // Calculate target positions
    float target_x = radius_mm * cos(angle);
    float target_y = radius_mm * sin(angle);

    // This is a simplified representation. For a real CNC, you'd use a
    // stepper library or Bresenham's algorithm to move to the target X/Y.
    // Here, we'll just pulse based on direction.
    
    // --- X Axis Stepper ---
    digitalWrite(DIR_PIN_1, target_x > 0 ? HIGH : LOW);
    digitalWrite(PULSE_PIN_1, HIGH);
    delayMicroseconds(step_delay_us / 2);
    digitalWrite(PULSE_PIN_1, LOW);
    delayMicroseconds(step_delay_us / 2);

    // --- Y Axis Stepper ---
    digitalWrite(DIR_PIN_2, target_y > 0 ? HIGH : LOW);
    digitalWrite(PULSE_PIN_2, HIGH);
    delayMicroseconds(step_delay_us / 2);
    digitalWrite(PULSE_PIN_2, LOW);
    delayMicroseconds(step_delay_us / 2);
  }
}

