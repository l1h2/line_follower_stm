#include "pid/base_pwm_pid.h"

#include <stdlib.h>

#include "timer/time.h"

#define KP 50
#define KI 0
#define KD 0
#define KFF 0
#define FRAME_INTERVAL 1  // ms

static BasePwmPid base_pwm_pid = {
    .kp = KP,
    .ki = KI,
    .kd = KD,
    .kff = KFF,
    .frame_interval = FRAME_INTERVAL,
    .last_pid_time = 0,
};

static const ErrorStruct* errors = NULL;

static inline int16_t get_p(void) {
    if (base_pwm_pid.kp == 0) return 0;

    return base_pwm_pid.kp * errors->error;
}

static inline int16_t get_i(void) {
    if (base_pwm_pid.ki == 0) return 0;

    return base_pwm_pid.ki * errors->error_sum * base_pwm_pid.frame_interval;
}

static inline int16_t get_d(void) {
    if (base_pwm_pid.kd == 0) return 0;

    return base_pwm_pid.kd * errors->delta_error / base_pwm_pid.frame_interval;
}

static inline int16_t get_ff(void) {
    if (base_pwm_pid.kff == 0) return 0;

    return base_pwm_pid.kff * errors->feedforward;
}

void init_base_pwm_pid(const ErrorStruct* error_struct) {
    errors = error_struct;
}

const BasePwmPid* get_base_pwm_pid_ptr(void) { return &base_pwm_pid; }

int16_t get_base_pwm_pid(void) {
    return get_ff() - (get_p() + get_i() + get_d());
}

bool update_pending_base_pwm_pid(void) {
    return time_elapsed(base_pwm_pid.last_pid_time,
                        base_pwm_pid.frame_interval);
}

void update_base_pwm_pid(void) { base_pwm_pid.last_pid_time = time(); }

void set_base_pwm_kp(const uint8_t kp) { base_pwm_pid.kp = kp; }

void set_base_pwm_ki(const uint8_t ki) { base_pwm_pid.ki = ki; }

void set_base_pwm_kd(const uint16_t kd) { base_pwm_pid.kd = kd; }

void set_base_pwm_kff(const uint8_t kff) { base_pwm_pid.kff = kff; }
