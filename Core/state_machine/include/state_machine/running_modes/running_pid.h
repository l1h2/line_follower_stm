#ifndef RUNNING_PID_H
#define RUNNING_PID_H

#include "../state_machine_base.h"

/**
 * @brief Handles the running PID mode logic.
 * @param sm Pointer to the state machine structure.
 */
void running_pid(const StateMachine* const sm);

/**
 * @brief Handles the transition from running PID mode to stopped state.
 */
void running_pid_to_stopped(void);

#endif  // RUNNING_PID_H
