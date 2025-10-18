#include "state_machine/states/stopped.h"

#include "logger/logger.h"
#include "pid/pid.h"
#include "pure_pursuit/pure_pursuit.h"
#include "sensors/sensors.h"
#include "state_machine/handlers/config_handler.h"
#include "state_machine/handlers/state_handler.h"
#include "track/track.h"

void handle_stopped(void) {
    debug_print("STOPPED State: Handling stopped logic");
    request_next_state(STATE_IDLE);
}

static void handle_stopped_to_idle(void) {
    debug_print("Transitioning from STOPPED to IDLE");
    restart_sensors();
    reset_track();
    restart_pid();
    restart_pure_pursuit();
    restart_state_machine();
}

bool handle_stopped_transitions(const RobotStates next_state) {
    switch (next_state) {
        case STATE_IDLE:
            handle_stopped_to_idle();
            break;
        default:
            return false;
    }

    return true;
}
