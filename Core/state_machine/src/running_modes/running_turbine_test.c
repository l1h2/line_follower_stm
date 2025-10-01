#include "state_machine/running_modes/running_turbine_test.h"

#include <stdint.h>

#include "logger/logger.h"
#include "serial/serial_in.h"
#include "turbine/turbine.h"

void running_turbine_test(const StateMachine* sm) {
    debug_print("RUNNING_TURBINE_TEST Mode: Handling running logic");

    uint16_t turbine_pwm = get_turbine_pwm();
    turn_on_turbine(0);

    while (sm->can_run) {
        debug_print_diagnostics();
        if (!process_serial_messages()) continue;

        const uint16_t current_pwm = get_turbine_pwm();

        if (current_pwm == 0) {
            debug_print(
                "Turbine PWM is 0, stopping turbine and exiting test mode");
            break;
        }

        if (current_pwm != turbine_pwm) {
            debug_print("Turbine PWM changed, updating turbine PWM");
            update_turbine_pwm(current_pwm);
        }
    }

    debug_print("Finalizing RUNNING_TURBINE_TEST mode");
}

void running_turbine_test_to_stopped(void) { turn_off_turbine(); }
