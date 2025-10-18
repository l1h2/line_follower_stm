#ifndef RUNNING_PURE_PURSUIT_H
#define RUNNING_PURE_PURSUIT_H

#include "../state_machine_base.h"

/**
 * @brief Handles the running pure pursuit mode logic.
 * @param sm Pointer to the state machine structure.
 */
void running_pure_pursuit(const StateMachine* const sm);

/**
 * @brief Handles the transition from running pure pursuit mode to stopped
 * state.
 */
void running_pure_pursuit_to_stopped(void);

#endif  // RUNNING_PURE_PURSUIT_H
