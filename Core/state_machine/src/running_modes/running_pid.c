#include "state_machine/running_modes/running_pid.h"

#include <stdbool.h>
#include <stdint.h>

#include "logger/logger.h"
#include "pid/pid.h"
#include "sensors/encoder.h"
#include "serial/serial_in.h"
#include "serial/serial_out.h"
#include "state_machine/handlers/config_handler.h"
#include "state_machine/running_modes/running_base.h"
#include "track/track.h"

void running_pid(const StateMachine* const sm) {
    debug_print("RUNNING_PID Mode: Handling running logic");

    const PidStruct* pid = get_pid();

    start_turbine_if_needed();
    set_start_time();

    while (sm->can_run) {
        if (!update_pid()) continue;

        check_stop(update_track(
            update_encoder_data_async(pid->speed_pid->frame_interval)));

        if (sm->log_data) send_message(OPERATION_DATA);
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
    stop_turbine_if_needed();
}
