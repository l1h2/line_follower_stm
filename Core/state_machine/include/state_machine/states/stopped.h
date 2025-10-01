#ifndef STOPPED_STATE_H
#define STOPPED_STATE_H

#include <stdbool.h>

#include "../state_machine_base.h"

/**
 * @brief Handles the stopped state logic.
 */
void handle_stopped(void);

/**
 * @brief Handles the transitions within the stopped state.
 * @param next_state The next state to transition to.
 * @return true if the transition was successful, false otherwise.
 */
bool handle_stopped_transitions(const RobotStates next_state);

#endif  // STOPPED_STATE_H
