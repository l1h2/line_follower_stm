#include "state_machine/state_machine.h"

#include <stdint.h>

#include "logger/logger.h"
#include "serial/serial_out.h"
#include "state_machine/handlers/config_handler.h"
#include "state_machine/handlers/state_handler.h"
#include "state_machine/handlers/transition_handler.h"

void run_state_machine(void) {
    debug_print("Starting state machine...");
    const StateMachine* const sm = get_state_machine();

    while (sm->current_state != STATE_ERROR) {
        send_message(STATE);
        handle_state(sm);

        if (sm->current_state != sm->next_state) {
            request_transition();
        }
    }

    send_message(STATE);
}
