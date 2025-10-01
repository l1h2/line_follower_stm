#ifndef RUNNING_TURBINE_TEST_H
#define RUNNING_TURBINE_TEST_H

#include "../state_machine_base.h"

/**
 * @brief Handles the running turbine test mode logic.
 * @param sm Pointer to the state machine structure.
 */
void running_turbine_test(const StateMachine* sm);

/**
 * @brief Handles the transition from running turbine test mode to stopped
 * state.
 */
void running_turbine_test_to_stopped(void);

#endif  // RUNNING_TURBINE_TEST_H
