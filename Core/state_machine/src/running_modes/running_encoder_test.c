#include "state_machine/running_modes/running_encoder_test.h"

#include "logger/logger.h"
#include "pid/pid.h"
#include "serial/serial_in.h"
#include "state_machine/running_modes/running_base.h"

#define ENCODER_TEST_DISTANCE_CM 100.0f

void running_encoder_test(const StateMachine* const sm) {
    debug_print("RUNNING_ENCODER_TEST Mode: Handling running logic");

    const PidStruct* pid = get_pid();

    start_turbine_if_needed();

    while (sm->can_run) {
        if (!update_speed_pid()) continue;

        if (pid->errors->sensors->encoders->distance >=
            ENCODER_TEST_DISTANCE_CM) {
            debug_print("Reached target distance for encoder test");
            break;
        }

        process_serial_messages();
    }

    debug_print("Finalizing RUNNING_ENCODER_TEST mode");
}

void running_encoder_test_to_stopped(void) { stop_turbine_if_needed(); }
