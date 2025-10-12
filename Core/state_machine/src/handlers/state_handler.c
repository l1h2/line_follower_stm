#include "state_machine/handlers/state_handler.h"

#include "logger/logger.h"
#include "state_machine/handlers/config_handler.h"

// States
#include "state_machine/states/error.h"
#include "state_machine/states/idle.h"
#include "state_machine/states/init.h"
#include "state_machine/states/running.h"
#include "state_machine/states/stopped.h"

static bool validate_next_state(const StateMachine* const sm) {
    return (sm->next_state == sm->current_state);
}

void handle_state(const StateMachine* const sm) {
    switch (sm->current_state) {
        case STATE_INIT:
            handle_init(sm);
            break;
        case STATE_IDLE:
            handle_idle(sm);
            break;
        case STATE_RUNNING:
            handle_running(sm);
            break;
        case STATE_STOPPED:
            handle_stopped();
            break;
        case STATE_ERROR:
            handle_error();
            break;
        default:
            debug_print("Unknown state!");
            return;
    }
}

bool request_next_state(const RobotStates next_state) {
    StateMachine* const sm = get_state_machine_ptr();

    if (!validate_next_state(sm)) return false;

    sm->next_state = next_state;
    return true;
}
