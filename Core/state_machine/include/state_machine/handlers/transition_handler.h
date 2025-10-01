#ifndef TRANSITION_HANDLER_H
#define TRANSITION_HANDLER_H

#include <stdbool.h>

/**
 * @brief Requests a state transition for the state machine.
 * @return true if the transition was successful, false otherwise.
 */
bool request_transition(void);

#endif  // TRANSITION_HANDLER_H
