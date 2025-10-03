#include "pid/controllers/speed_pid.h"

#include <stdlib.h>

#include "sensors/sensors_base.h"
#include "timer/time.h"

#define KP 50
#define KI 0
#define KD 0
#define FRAME_INTERVAL 10  // ms

static BaseSpeedPid base_pid = {
    .kp = KP,
    .ki = KI,
    .kd = KD,
    .frame_interval = FRAME_INTERVAL,
    .last_pid_time = 0,
};

static const SpeedErrors* speed_errors = NULL;

static struct {
    int16_t left_p;
    int16_t right_p;
    int16_t left_i;
    int16_t right_i;
    int16_t left_d;
    int16_t right_d;
    int16_t left_pid;
    int16_t right_pid;
} pid_struct = {0};

static inline void update_p(void) {
    if (base_pid.kp == 0) return;

    pid_struct.left_p = base_pid.kp * speed_errors->left_error;
    pid_struct.right_p = base_pid.kp * speed_errors->right_error;
}

static inline void update_i(void) {
    if (base_pid.ki == 0) return;

    pid_struct.left_i =
        base_pid.ki * speed_errors->left_error_sum * base_pid.frame_interval;
    pid_struct.right_i =
        base_pid.ki * speed_errors->right_error_sum * base_pid.frame_interval;
}

static inline void update_d(void) {
    if (base_pid.kd == 0) return;

    pid_struct.left_d =
        base_pid.kd * speed_errors->left_delta_error / base_pid.frame_interval;
    pid_struct.right_d =
        base_pid.kd * speed_errors->right_delta_error / base_pid.frame_interval;
}

static inline void update_pid(void) {
    update_p();
    update_i();
    update_d();

    pid_struct.left_pid =
        pid_struct.left_p + pid_struct.left_i + pid_struct.left_d;
    pid_struct.right_pid =
        pid_struct.right_p + pid_struct.right_i + pid_struct.right_d;
}

void init_base_speed_pid(const ErrorStruct* error_struct) {
    speed_errors = error_struct->speed_errors;
}

const BaseSpeedPid* get_base_speed_pid_ptr(void) { return &base_pid; }

void get_base_speed_pid(int16_t* left_pwm, int16_t* right_pwm) {
    update_pid();

    if (left_pwm != NULL) *left_pwm = pid_struct.left_pid;
    if (right_pwm != NULL) *right_pwm = pid_struct.right_pid;
}

bool update_pending_base_speed_pid(void) {
    return time_elapsed(base_pid.last_pid_time, base_pid.frame_interval);
}

void update_base_speed_pid(void) { base_pid.last_pid_time = time(); }

void set_base_speed_kp(const uint8_t kp) { base_pid.kp = kp; }

void set_base_speed_ki(const uint8_t ki) { base_pid.ki = ki; }

void set_base_speed_kd(const uint16_t kd) { base_pid.kd = kd; }
