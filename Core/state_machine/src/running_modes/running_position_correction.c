#include "state_machine/running_modes/running_position_correction.h"

#include "logger/logger.h"
#include "pid/pid.h"
#include "pure_pursuit/pure_pursuit.h"
#include "serial/serial_in.h"
#include "serial/serial_out.h"
#include "state_machine/handlers/config_handler.h"
#include "state_machine/running_modes/running_base.h"
#include "track/track.h"

void running_position_correction(const StateMachine* const sm) {
    debug_print("RUNNING_POSITION_CORRECTION Mode: Handling running logic");

    start_turbine_if_needed();
    set_start_time();

    while (sm->can_run) {
        if (!update_peripheral_sensors()) continue;

        check_stop(update_track(false));
        process_serial_messages();

        if (!update_pure_pursuit()) continue;

        if (sm->log_data) send_message(OPERATION_DATA);
    }

    debug_print("Finalizing RUNNING_POSITION_CORRECTION mode");
}

void running_position_correction_to_stopped(void) {
    const PidStruct* pid = get_pid();

    const uint16_t max_pwm_save = pid->max_pwm;
    uint16_t max_pwm = max_pwm_save;

    while (pid->max_pwm) {
        if (!update_pid()) continue;
        set_max_pwm(--max_pwm);
    }

    set_max_pwm(max_pwm_save);
    stop_turbine_if_needed();
}
