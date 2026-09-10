#ifndef RUNNING_POSITION_CORRECTION_H
#define RUNNING_POSITION_CORRECTION_H

#include "../state_machine_base.h"

/**
 * @brief Handles the running position correction mode logic.
 * @param sm Pointer to the state machine structure.
 */
void running_position_correction(const StateMachine* const sm);

/**
 * @brief Handles the transition from running position correction mode to
 * stopped state.
 */
void running_position_correction_to_stopped(void);

#endif  // RUNNING_POSITION_CORRECTION_H
