#include "motors/motors.h"

#include <stdlib.h>

#include "hal/pwm.h"

void init_motors(void) { init_pwm(); }

uint16_t get_max_pwm(void) { return MAX_PWM; }

uint16_t get_min_pwm(void) { return MIN_PWM; }

void set_motor_left(const int16_t pwm) {
    set_motor_left_dir(pwm > 0);
    set_pwm(MOTOR_LEFT, (uint16_t)abs(pwm));
}

void set_motor_right(const int16_t pwm) {
    set_motor_right_dir(pwm > 0);
    set_pwm(MOTOR_RIGHT, (uint16_t)abs(pwm));
}

void set_motors(const int16_t left_pwm, const int16_t right_pwm) {
    set_motor_left(left_pwm);
    set_motor_right(right_pwm);
}

uint16_t get_motor_left_pwm(void) { return get_pwm(MOTOR_LEFT); }

uint16_t get_motor_right_pwm(void) { return get_pwm(MOTOR_RIGHT); }

void get_motors_pwm(uint16_t* left_pwm, uint16_t* right_pwm) {
    if (left_pwm != NULL) *left_pwm = get_motor_left_pwm();
    if (right_pwm != NULL) *right_pwm = get_motor_right_pwm();
}

void stop_motors(void) {
    set_motor_left(0);
    set_motor_right(0);
}
