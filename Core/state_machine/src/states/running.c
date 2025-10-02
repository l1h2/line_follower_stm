#include "state_machine/states/running.h"

#include "logger/logger.h"
#include "sensors/sensors.h"
#include "state_machine/handlers/config_handler.h"
#include "state_machine/handlers/state_handler.h"
#include "turbine/turbine.h"

// Running modes
#include "state_machine/running_modes/running_pid.h"
#include "state_machine/running_modes/running_sensor_test.h"
#include "state_machine/running_modes/running_turbine_test.h"

void handle_running(const StateMachine* sm) {
    debug_print("RUNNING State: Choosing running mode");
    restart_sensors();

    switch (sm->running_mode) {
        case RUNNING_INIT:
            debug_print("No running mode set, going back to IDLE state");
            request_next_state(STATE_IDLE);
            return;
        case RUNNING_SENSOR_TEST:
            debug_print("Running mode set to RUNNING_SENSOR_TEST");
            running_sensor_test(sm);
            break;
        case RUNNING_TURBINE_TEST:
            debug_print("Running mode set to RUNNING_TURBINE_TEST");
            if (!turbine_can_run()) {
                debug_print("Turbine PWM is 0, cannot run turbine test");
                request_next_state(STATE_IDLE);
                return;
            }
            running_turbine_test(sm);
            break;
        case RUNNING_PID:
            debug_print("Running mode set to RUNNING_PID");
            running_pid(sm);
            break;
        default:
            debug_print("Unknown running mode set, going back to IDLE state");
            request_next_state(STATE_IDLE);
            return;
    }

    debug_print("Finalizing RUNNING state, preparing to stop");
    request_next_state(STATE_STOPPED);
}

static inline bool handle_running_to_idle(void) {
    debug_print("Transitioning from RUNNING to IDLE");
    set_can_run(false);
    return true;
}

static bool handle_running_to_stopped(const RunningModes running_mode) {
    debug_print("Transitioning from RUNNING to STOPPED");

    switch (running_mode) {
        case RUNNING_PID:
            running_pid_to_stopped();
            break;
        case RUNNING_SENSOR_TEST:
            running_sensor_test_to_stopped();
            break;
        case RUNNING_TURBINE_TEST:
            running_turbine_test_to_stopped();
            break;
        default:
            debug_print("Unknown running mode, going to error state");
            return false;
    }

    debug_print("Base PWM set to 0, stopping motors");
    return true;
}

bool handle_running_transitions(const RobotStates next_state,
                                const RunningModes running_mode) {
    switch (next_state) {
        case STATE_IDLE:
            return handle_running_to_idle();
        case STATE_STOPPED:
            return handle_running_to_stopped(running_mode);
        default:
            return false;
    }
}
