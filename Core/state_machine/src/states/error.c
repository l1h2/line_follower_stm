#include "state_machine/states/error.h"

#include "logger/logger.h"
#include "state_machine/handlers/state_handler.h"

void handle_error(void) { debug_print("ERROR State: Handling error logic"); }

void handle_any_to_error(void) {
    debug_print("Transitioning to ERROR State from any state");
    request_next_state(STATE_ERROR);
}

bool handle_error_transitions(const RobotStates next_state) {
    switch (next_state) {
        default:
            return false;
    }

    return true;
}
