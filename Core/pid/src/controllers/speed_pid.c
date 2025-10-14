#include "pid/controllers/speed_pid.h"

#include <stdlib.h>

#include "sensors/sensors_base.h"
#include "timer/time.h"

#define KP 10
#define KI 10
#define KD 0
#define KFF 0
#define FRAME_INTERVAL 10  // ms

#define BASE_SPEED 20.0f  // cm/s
#define PWM_MAX_DELTA 100

static BaseSpeedPid base_pid = {
    .kp = KP,
    .ki = KI,
    .kd = KD,
    .kff = KFF,
    .base_speed = BASE_SPEED,
    .frame_interval = FRAME_INTERVAL,
    .last_pid_time = 0,
};

static const SpeedErrors* speed_errors = NULL;

static struct {
    float left_p;
    float right_p;
    float left_i;
    float right_i;
    float left_d;
    float right_d;
    float left_ff;
    float right_ff;
    int16_t last_left_pwm;
    int16_t last_right_pwm;
    int16_t left_pwm;
    int16_t right_pwm;
} pid_struct = {0};

static inline void update_p(void) {
    if (base_pid.kp == 0) return;

    pid_struct.left_p = base_pid.kp * speed_errors->left_error;
    pid_struct.right_p = base_pid.kp * speed_errors->right_error;
}

static inline void update_i(void) {
    if (base_pid.ki == 0) return;

    const float new_ki = base_pid.ki / 100.0f;

    pid_struct.left_i =
        new_ki * speed_errors->left_error_sum * base_pid.frame_interval;
    pid_struct.right_i =
        new_ki * speed_errors->right_error_sum * base_pid.frame_interval;
}

static inline void update_d(void) {
    if (base_pid.kd == 0) return;

    pid_struct.left_d =
        base_pid.kd * speed_errors->left_delta_error / base_pid.frame_interval;
    pid_struct.right_d =
        base_pid.kd * speed_errors->right_delta_error / base_pid.frame_interval;
}

static inline void update_ff(void) {
    if (base_pid.kff == 0) return;

    pid_struct.left_ff = base_pid.kff * speed_errors->left_target_speed;
    pid_struct.right_ff = base_pid.kff * speed_errors->right_target_speed;
}

static inline void update_pwm(void) {
    float left_out = pid_struct.left_p + pid_struct.left_i + pid_struct.left_d;
    float right_out =
        pid_struct.right_p + pid_struct.right_i + pid_struct.right_d;

    if (left_out > 1000.0f) {
        left_out = 1000.0f;
    } else if (right_out < -1000.0f) {
        right_out = -1000.0f;
    }

    if (right_out > 1000.0f) {
        right_out = 1000.0f;
    } else if (right_out < -1000.0f) {
        right_out = -1000.0f;
    }

    // pid_struct.left_pwm = left_out + pid_struct.left_ff;
    // pid_struct.right_pwm = right_out + pid_struct.right_ff;

    // const int16_t left_delta = left_out - pid_struct.last_left_pwm;
    // const int16_t right_delta = right_out - pid_struct.last_right_pwm;

    // pid_struct.left_pwm = left_delta > PWM_MAX_DELTA
    //                           ? pid_struct.last_left_pwm + PWM_MAX_DELTA
    //                           : left_out;
    // pid_struct.right_pwm = right_delta > PWM_MAX_DELTA
    //                            ? pid_struct.last_right_pwm + PWM_MAX_DELTA
    //                            : right_out;

    pid_struct.left_pwm = (int16_t)left_out;
    pid_struct.right_pwm = (int16_t)right_out;

    pid_struct.last_left_pwm = pid_struct.left_pwm;
    pid_struct.last_right_pwm = pid_struct.right_pwm;
}

const BaseSpeedPid* init_base_speed_pid(const ErrorStruct* const error_struct) {
    speed_errors = error_struct->speed_errors;
    return &base_pid;
}

const BaseSpeedPid* get_base_speed_pid_ptr(void) { return &base_pid; }

void get_base_speed_pid(int16_t* const left_pwm, int16_t* const right_pwm) {
    update_p();
    update_i();
    update_d();
    update_ff();
    update_pwm();

    if (left_pwm != NULL) *left_pwm = pid_struct.left_pwm;
    if (right_pwm != NULL) *right_pwm = pid_struct.right_pwm;
}

bool update_pending_base_speed_pid(void) {
    return time_elapsed(base_pid.last_pid_time, base_pid.frame_interval);
}

void update_base_speed_pid(void) { base_pid.last_pid_time = time(); }

void set_base_speed_kp(const uint16_t kp) { base_pid.kp = kp; }

void set_base_speed_ki(const uint16_t ki) { base_pid.ki = ki; }

void set_base_speed_kd(const uint16_t kd) { base_pid.kd = kd; }

void set_base_speed_kff(const uint16_t kff) { base_pid.kff = kff; }

void set_base_speed(const float speed) { base_pid.base_speed = speed; }
