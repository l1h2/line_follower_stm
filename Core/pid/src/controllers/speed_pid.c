#include "pid/controllers/speed_pid.h"

#include <stdlib.h>

#include "motors/motors.h"
#include "sensors/sensors_base.h"
#include "timer/time.h"

#define KP 10
#define KI 0.1f
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
    uint16_t max_pwm;
    int16_t min_pwm;
} pid_struct = {0};

static inline void update_p(void) {
    pid_struct.left_p = base_pid.kp * speed_errors->left_error;
    pid_struct.right_p = base_pid.kp * speed_errors->right_error;
}

static inline void update_i(void) {
    pid_struct.left_i =
        base_pid.ki * speed_errors->left_error_sum * base_pid.frame_interval;
    pid_struct.right_i =
        base_pid.ki * speed_errors->right_error_sum * base_pid.frame_interval;
}

static inline void update_d(void) {
    pid_struct.left_d =
        base_pid.kd * speed_errors->left_delta_error / base_pid.frame_interval;
    pid_struct.right_d =
        base_pid.kd * speed_errors->right_delta_error / base_pid.frame_interval;
}

static inline void update_ff(void) {
    pid_struct.left_ff = base_pid.kff * speed_errors->left_delta_target_speed /
                         base_pid.frame_interval;
    pid_struct.right_ff = base_pid.kff *
                          speed_errors->right_delta_target_speed /
                          base_pid.frame_interval;
}

static inline void update_pwm(void) {
    float left_out = pid_struct.left_p + pid_struct.left_i + pid_struct.left_d +
                     pid_struct.left_ff;
    float right_out = pid_struct.right_p + pid_struct.right_i +
                      pid_struct.right_d + pid_struct.right_ff;

    if (left_out > pid_struct.max_pwm) {
        left_out = pid_struct.max_pwm;
    } else if (left_out < pid_struct.min_pwm) {
        left_out = pid_struct.min_pwm;
    }

    if (right_out > pid_struct.max_pwm) {
        right_out = pid_struct.max_pwm;
    } else if (right_out < pid_struct.min_pwm) {
        right_out = pid_struct.min_pwm;
    }

    pid_struct.left_pwm = (int16_t)left_out;
    pid_struct.right_pwm = (int16_t)right_out;

    pid_struct.last_left_pwm = pid_struct.left_pwm;
    pid_struct.last_right_pwm = pid_struct.right_pwm;
}

const BaseSpeedPid* init_base_speed_pid(const ErrorStruct* const error_struct) {
    pid_struct.max_pwm = get_max_pwm();
    pid_struct.min_pwm = -pid_struct.max_pwm;

    speed_errors = error_struct->speed_errors;
    return &base_pid;
}

const BaseSpeedPid* get_base_speed_pid_ptr(void) { return &base_pid; }

void update_base_speed_pid(void) {
    update_p();
    update_i();
    update_d();
    update_ff();
    update_pwm();
    set_motors(pid_struct.left_pwm, pid_struct.right_pwm);
}

bool update_pending_base_speed_pid(void) {
    return time_elapsed(base_pid.last_pid_time, base_pid.frame_interval);
}

void update_base_speed_pid_time(void) { base_pid.last_pid_time = time(); }

void set_base_speed_kp(const uint16_t kp) { base_pid.kp = kp; }

void set_base_speed_ki(const float ki) { base_pid.ki = ki; }

void set_base_speed_kd(const uint16_t kd) { base_pid.kd = kd; }

void set_base_speed_kff(const uint16_t kff) { base_pid.kff = kff; }

void set_base_speed(const float speed) { base_pid.base_speed = speed; }
