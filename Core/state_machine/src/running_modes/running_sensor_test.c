#include "state_machine/running_modes/running_sensor_test.h"

#include "logger/logger.h"
#include "serial/serial_in.h"

void running_sensor_test(const StateMachine* sm) {
    debug_print("RUNNING_SENSOR_TEST Mode: Handling running logic");

    while (sm->can_run) {
        debug_print_diagnostics();
        process_serial_messages();
    }

    debug_print("Finalizing RUNNING_SENSOR_TEST mode");
}

void running_sensor_test_to_stopped(void) {
    // No actions needed when transitioning to stopped state
}
