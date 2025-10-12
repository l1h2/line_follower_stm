#include "state_machine/handlers/transition_handler.h"

#include "state_machine/handlers/config_handler.h"

// States
#include "state_machine/states/error.h"
#include "state_machine/states/idle.h"
#include "state_machine/states/init.h"
#include "state_machine/states/running.h"
#include "state_machine/states/stopped.h"

static bool validate_transition(const StateMachine* const sm) {
    return (sm->next_state != sm->current_state);
}

static bool handle_transition(const StateMachine* const sm) {
    switch (sm->current_state) {
        case STATE_INIT:
            return handle_init_transitions(sm->next_state);
        case STATE_IDLE:
            return handle_idle_transitions(sm->next_state);
        case STATE_RUNNING:
            return handle_running_transitions(sm->next_state, sm->running_mode);
        case STATE_STOPPED:
            return handle_stopped_transitions(sm->next_state);
        case STATE_ERROR:
            return handle_error_transitions(sm->next_state);
        default:
            return false;
    }
}

static void update_state(StateMachine* const sm) {
    sm->previous_state = sm->current_state;
    sm->current_state = sm->next_state;
}

bool request_transition(void) {
    StateMachine* const sm = get_state_machine_ptr();

    if (!validate_transition(sm)) return false;

    if (handle_transition(sm)) {
        update_state(sm);
        return true;
    }

    handle_any_to_error();
    update_state(sm);
    return false;
}
