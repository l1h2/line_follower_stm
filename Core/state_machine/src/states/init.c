#include "state_machine/states/init.h"

#include "logger/logger.h"
#include "motors/motors.h"
#include "pid/pid.h"
#include "sensors/sensors.h"
#include "serial/serial_base.h"
#include "state_machine/handlers/state_handler.h"
#include "timer/time.h"

void handle_init(void) {
    debug_print("INIT State: Initializing state machine");

    init_serial();
    init_timer();
    init_sensors();
    init_motors();
    init_pid();

    request_next_state(STATE_IDLE);
}

static void handle_init_to_idle(void) {
    debug_print("Transitioning from INIT to IDLE");
}

bool handle_init_transitions(const RobotStates next_state) {
    switch (next_state) {
        case STATE_IDLE:
            handle_init_to_idle();
            break;
        default:
            return false;
    }

    return true;
}
