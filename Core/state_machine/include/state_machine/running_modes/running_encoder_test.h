#ifndef RUNNING_ENCODER_TEST_H
#define RUNNING_ENCODER_TEST_H

#include "../state_machine_base.h"

/**
 * @brief Handles the running encoder test mode logic.
 * @param sm Pointer to the state machine structure.
 */
void running_encoder_test(const StateMachine* const sm);

/**
 * @brief Handles the transition from running encoder test mode to stopped
 * state.
 */
void running_encoder_test_to_stopped(void);

#endif  // RUNNING_ENCODER_TEST_H
