#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include <stdbool.h>
#include <stdint.h>

#include "../state_machine_base.h"

/**
 * @brief Resets the state machine to its initial state.
 */
void reset_state_machine(void);

/**
 * @brief Restarts the state machine, allowing it to run again.
 */
void restart_state_machine(void);

/**
 * @brief Gets the current state machine instance.
 * @return Pointer to the current state machine instance.
 */
const StateMachine* get_state_machine(void);

/**
 * @brief Gets a pointer to the current state machine instance.
 * @return Pointer to the current state machine instance.
 * @note This function is used to modify the state machine instance directly.
 * @warning This function should only be used by other state machine handlers.
 */
StateMachine* get_state_machine_ptr(void);

/**
 * @brief Sets the running mode of the state machine.
 * @param mode The running mode to set.
 */
void set_running_mode(const RunningModes mode);

/**
 * @brief Sets the stopping mode of the state machine.
 * @param mode The stopping mode to set.
 */
void set_stop_mode(const StopModes mode);

/**
 * @brief Sets the can_run flag for the state machine.
 * @param can_run The value to set for the can_run flag.
 */
void set_can_run(const bool can_run);

/**
 * @brief Sets the number of laps for the state machine.
 * @param lap The number of laps to set.
 */
void set_laps(const uint8_t laps);

/**
 * @brief Adds a lap to the current lap count of the state machine.
 */
void add_lap(void);

/**
 * @brief Sets the current lap count of the state machine.
 * @param lap The lap number to be set.
 */
void set_lap(const uint8_t lap);

/**
 * @brief Starts the time for the state machine operation.
 * @param start_time The start time to set.
 */
void set_start_time(void);

/**
 * @brief Sets the stop time for the state machine.
 * @param stop_time The stop time to set in seconds.
 */
void set_stop_time(const uint8_t stop_time);

/**
 * @brief Sets the stop distance for the state machine.
 * @param stop_distance The stop distance to set in centimeters.
 */
void set_stop_distance(const uint16_t stop_distance);

/**
 * @brief Sets the log_data flag for the state machine.
 * @param log_data The value to set for the log_data flag.
 */
void set_log_data(const bool log_data);

#endif  // CONFIG_HANDLER_H
