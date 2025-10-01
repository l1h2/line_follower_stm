#include "state_machine/running_modes/running_pid.h"

#include "logger/logger.h"
#include "pid/pid.h"
#include "serial/serial_in.h"
#include "state_machine/handlers/config_handler.h"
#include "state_machine/running_modes/running_base.h"

void running_pid(const StateMachine* sm) {
    debug_print("RUNNING_PID Mode: Handling running logic");

    restart_pwm();
    start_turbine_if_needed();

    set_start_time();

    while (sm->can_run) {
        if (!update_pid()) continue;

        process_serial_messages();
    }

    debug_print("Finalizing RUNNING_PID mode");
}

void running_pid_to_stopped(void) {
    const PidStruct* pid = get_pid();

    const uint8_t max_pwm_save = pid->max_pwm;
    uint8_t max_pwm = max_pwm_save;

    while (pid->max_pwm) {
        if (!update_pid()) continue;

        set_max_pwm(--max_pwm);
    }

    set_max_pwm(max_pwm_save);
    restart_pwm();
    stop_turbine_if_needed();
}
