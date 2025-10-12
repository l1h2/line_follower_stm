#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include <stdbool.h>

#include "../state_machine_base.h"

/**
 * @brief Handles the idle state logic.
 * @param sm Pointer to the state machine structure.
 */
void handle_idle(const StateMachine* const sm);

/**
 * @brief Handles the transitions within the idle state.
 * @param next_state The next state to transition to.
 * @return true if the transition was successful, false otherwise.
 */
bool handle_idle_transitions(const RobotStates next_state);

#endif  // IDLE_STATE_H
