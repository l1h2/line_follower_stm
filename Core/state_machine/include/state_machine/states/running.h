#ifndef RUNNING_STATE_H
#define RUNNING_STATE_H

#include <stdbool.h>
#include <stdint.h>

#include "../state_machine_base.h"

/**
 * @brief Handles the running state logic.
 * @param sm Pointer to the state machine structure.
 */
void handle_running(const StateMachine* const sm);

/**
 * @brief Handles the transitions within the running state.
 * @param next_state The next state to transition to.
 * @param running_mode The current running mode set for the robot.
 * @return true if the transition was successful, false otherwise.
 */
bool handle_running_transitions(const RobotStates next_state,
                                const RunningModes running_mode);

#endif  // RUNNING_STATE_H
