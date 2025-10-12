#include "state_machine/states/idle.h"

#include "logger/logger.h"
#include "serial/serial_in.h"
#include "serial/serial_out.h"
#include "state_machine/handlers/state_handler.h"

void handle_idle(const StateMachine* const sm) {
    debug_print("IDLE State: Waiting for bluetooth commands");

    while (!sm->can_run) {
        send_all_messages_async(LOG_INTERVAL);
        process_serial_messages();
    }

    debug_print("Start command received in IDLE State");
    request_next_state(STATE_RUNNING);
}

static void handle_idle_to_running(void) {
    debug_print("Transitioning from IDLE to RUNNING");
    send_all_messages_async(LOG_INTERVAL);
}

bool handle_idle_transitions(const RobotStates next_state) {
    switch (next_state) {
        case STATE_RUNNING:
            handle_idle_to_running();
            break;
        default:
            return false;
    }

    return true;
}
