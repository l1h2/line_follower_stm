#include "turbine/turbine.h"

#include "hal/pwm.h"
#include "timer/time.h"

static uint16_t turbine_pwm = 0;
static bool turbine_waiting = false;
static uint32_t turbine_start_time = 0;

void set_turbine_pwm(const uint16_t pwm) {
    if (pwm > MAX_PWM) {
        turbine_pwm = MAX_PWM;
    } else if (pwm < MIN_PWM) {
        turbine_pwm = MIN_PWM;
    } else {
        turbine_pwm = pwm;
    }
}

uint16_t get_turbine_pwm(void) { return turbine_pwm; }

void update_turbine_pwm(const uint16_t pwm) {
    set_turbine_pwm(pwm);
    set_pwm(MOTOR_TURBINE, turbine_pwm);
}

bool turbine_can_run(void) { return turbine_pwm > 0; }

void turn_on_turbine(const uint32_t wait) {
    if (turbine_pwm == 0) return;

    set_pwm(MOTOR_TURBINE, turbine_pwm);

    if (wait > 0) delay(wait);
}

bool turn_on_turbine_async(const uint32_t wait) {
    if (turbine_pwm == 0) return false;

    if (!turbine_waiting) {
        turbine_start_time = time();
        set_pwm(MOTOR_TURBINE, turbine_pwm);
        turbine_waiting = true;
        return false;
    }

    if (!time_elapsed(turbine_start_time, wait)) return false;

    turbine_waiting = false;
    return true;
}

void turn_off_turbine(void) { set_pwm(MOTOR_TURBINE, 0); }
