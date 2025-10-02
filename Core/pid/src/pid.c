#include "pid/pid.h"

#include <stdlib.h>

#include "motors/motors.h"
#include "pid/errors.h"
#include "timer/time.h"

#define KP 80                 // Proportional gain
#define KI 0                  // Integral gain
#define KD 4000               // Derivative gain
#define KFF 0                 // Feedforward gain
#define KB 0                  // Brake gain
#define INITIAL_PWM 200       // Initial PWM value to avoid acceleration issues
#define BASE_PWM 300          // Base PWM value for the motors
#define PID_FRAME_INTERVAL 1  // PID frame interval in milliseconds

static PidStruct pid = {
    .kp = KP,
    .ki = KI,
    .kd = KD,
    .kff = KFF,
    .kb = KB,
    .base_pwm = BASE_PWM,
    .current_pwm = INITIAL_PWM < BASE_PWM ? INITIAL_PWM : BASE_PWM,
    .max_pwm = 0,
    .min_pwm = 0,
    .frame_interval = PID_FRAME_INTERVAL,
    .last_pid_time = 0,
    .errors = 0,
};

static bool is_updating_errors = false;

static inline int16_t get_p(void) {
    if (pid.kp == 0) return 0;

    return pid.kp * pid.errors->error;
}

static inline int16_t get_i(void) {
    if (pid.ki == 0) return 0;

    return pid.ki * pid.errors->error_sum * pid.frame_interval;
}

static inline int16_t get_d(void) {
    if (pid.kd == 0) return 0;

    return pid.kd * pid.errors->delta_error / pid.frame_interval;
}

static inline int16_t get_delta_pwm(void) {
    return get_p() + get_i() + get_d();
}

static inline int16_t get_new_pwm(const int16_t delta_pwm) {
    int16_t pwm = pid.current_pwm + delta_pwm;

    if (pwm > pid.max_pwm) {
        return pid.max_pwm;
    } else if (pwm < pid.min_pwm) {
        return pid.min_pwm;
    }

    return pwm;
}

static void update_motors(const int16_t delta_pwm) {
    const int16_t pwm_a = get_new_pwm(delta_pwm);
    const int16_t pwm_b = get_new_pwm(-delta_pwm);

    set_motors(pwm_a, pwm_b);
}

void init_pid(void) {
    pid.max_pwm = get_max_pwm();
    pid.min_pwm = -pid.max_pwm;

    if (pid.errors == 0) {
        init_errors();
        pid.errors = get_errors();
    }
}

const PidStruct* get_pid(void) { return &pid; }

bool update_pid(void) {
    if (!is_updating_errors &&
        !time_elapsed(pid.last_pid_time, pid.frame_interval)) {
        return false;
    }

    if (!is_updating_errors) pid.last_pid_time = time();

    if (!update_errors_async()) {
        is_updating_errors = true;
        return false;
    }

    is_updating_errors = false;
    update_motors(get_delta_pwm());
    return true;
}

void set_kp(const uint8_t kp) { pid.kp = kp; }

void set_ki(const uint8_t ki) { pid.ki = ki; }

void set_kd(const uint16_t kd) { pid.kd = kd; }

void reset_pwm(void) { pid.current_pwm = pid.base_pwm; }

void restart_pwm(void) {
    pid.current_pwm = INITIAL_PWM < pid.base_pwm ? INITIAL_PWM : pid.base_pwm;
}

void set_base_pwm(const uint16_t pwm) {
    const uint16_t min_pwm = get_min_pwm();

    if (pwm >= pid.max_pwm) {
        pid.base_pwm = pid.max_pwm;
    } else if (pwm <= min_pwm) {
        pid.base_pwm = min_pwm;
    } else {
        pid.base_pwm = pwm;
    }

    reset_pwm();
}

void set_current_pwm(const int16_t pwm) {
    if (pwm >= pid.max_pwm) {
        pid.current_pwm = pid.max_pwm;
    } else if (pwm <= pid.min_pwm) {
        pid.current_pwm = pid.min_pwm;
    } else {
        pid.current_pwm = pwm;
    }
}

void set_max_pwm(const uint16_t pwm) {
    const uint16_t max_pwm = get_max_pwm();
    const uint16_t min_pwm = get_min_pwm();

    if (pwm > max_pwm) {
        pid.max_pwm = max_pwm;
    } else if (pwm < min_pwm) {
        pid.max_pwm = min_pwm;
    } else {
        pid.max_pwm = pwm;
    }

    pid.min_pwm = -pid.max_pwm;
}
