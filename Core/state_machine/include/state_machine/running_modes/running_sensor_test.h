#ifndef RUNNING_SENSOR_TEST_H
#define RUNNING_SENSOR_TEST_H

#include "../state_machine_base.h"

/**
 * @brief Handles the running sensor test mode logic.
 * @param sm Pointer to the state machine structure.
 */
void running_sensor_test(const StateMachine* const sm);

/**
 * @brief Handles the transition from running sensor test mode to stopped state.
 */
void running_sensor_test_to_stopped(void);

#endif  // RUNNING_SENSOR_TEST_H
