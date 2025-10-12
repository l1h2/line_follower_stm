#ifndef INIT_STATE_H
#define INIT_STATE_H

#include <stdbool.h>

#include "../state_machine_base.h"

/**
 * @brief Handles the initialization state logic.
 * @param sm Pointer to the StateMachine struct.
 */
void handle_init(const StateMachine* const sm);

/**
 * @brief Handles the transitions within the initialization state.
 * @param next_state The next state to transition to.
 * @return true if the transition was successful, false otherwise.
 */
bool handle_init_transitions(const RobotStates next_state);

#endif  // INIT_STATE_H
