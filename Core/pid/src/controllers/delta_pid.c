#include "pid/controllers/delta_pid.h"

#include <stdlib.h>

#include "timer/time.h"

#define KP 80
#define KI 0
#define KD 4000
#define FRAME_INTERVAL 1  // ms

static DeltaPid delta_pid = {
    .kp = KP,
    .ki = KI,
    .kd = KD,
    .frame_interval = FRAME_INTERVAL,
    .last_pid_time = 0,
};

static const ErrorStruct* errors = NULL;

static inline int16_t get_p(void) {
    if (delta_pid.kp == 0) return 0;

    return delta_pid.kp * errors->error;
}

static inline int16_t get_i(void) {
    if (delta_pid.ki == 0) return 0;

    return delta_pid.ki * errors->error_sum * delta_pid.frame_interval;
}

static inline int16_t get_d(void) {
    if (delta_pid.kd == 0) return 0;

    return delta_pid.kd * errors->delta_error / delta_pid.frame_interval;
}

void init_delta_pwm_pid(const ErrorStruct* error_struct) {
    errors = error_struct;
}

const DeltaPid* get_delta_pwm_pid_ptr(void) { return &delta_pid; }

int16_t get_delta_pwm_pid(void) { return get_p() + get_i() + get_d(); }

bool update_pending_delta_pwm_pid(void) {
    return time_elapsed(delta_pid.last_pid_time, delta_pid.frame_interval);
}

void update_delta_pwm_pid(void) { delta_pid.last_pid_time = time(); }

void set_delta_pwm_kp(const uint8_t kp) { delta_pid.kp = kp; }

void set_delta_pwm_ki(const uint8_t ki) { delta_pid.ki = ki; }

void set_delta_pwm_kd(const uint16_t kd) { delta_pid.kd = kd; }
