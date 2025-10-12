#ifndef STATE_MACHINE_BASE_H
#define STATE_MACHINE_BASE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @enum RunningModes
 * @brief Enumeration of running modes for the robot.
 */
typedef enum {
    RUNNING_INIT,          // Initial running mode
    RUNNING_SENSOR_TEST,   // Sensor testing mode
    RUNNING_TURBINE_TEST,  // Turbine testing mode
    RUNNING_ENCODER_TEST,  // Encoder testing mode
    RUNNING_PID,           // PID control mode
    RUNNING_PURE_PURSUIT   // Pure pursuit mode
} RunningModes;

/**
 * @enum StopModes
 * @brief Enumeration of stopping modes for the robot.
 */
typedef enum {
    STOP_MODE_NONE,      // No stopping mode
    STOP_MODE_TIME,      // Stop after a certain time
    STOP_MODE_LAPS,      // Stop after completing a number of laps
    STOP_MODE_DISTANCE,  // Stop after covering a certain distance
} StopModes;

/**
 * @enum RobotStates
 * @brief Enumeration of robot states for the state machine.
 */
typedef enum {
    STATE_INIT,     // Initial state of the robot
    STATE_IDLE,     // Idle state, waiting for commands
    STATE_RUNNING,  // Running state, executing laps
    STATE_STOPPED,  // Stopped state, waiting for restart
    STATE_ERROR     // Error state, handling errors
} RobotStates;

/**
 * @struct StateMachine
 * @brief Structure representing the state machine.
 */
typedef struct {
    RobotStates previous_state;  // Previous state of the robot
    RobotStates current_state;   // Current state of the robot
    RobotStates next_state;      // Next state of the robot
    RunningModes running_mode;   // Current running mode of the robot
    StopModes stop_mode;         // Current stopping mode of the robot
    bool can_run;                // Flag indicating if the robot can run
    uint8_t lap;                 // Current number of laps completed
    uint8_t laps;                // Number of laps to run
    uint32_t start_time;         // Time robot started running
    uint32_t stop_time;          // Time in milliseconds for the robot to stop
    uint16_t stop_distance;      // Distance in cm for the robot to stop
    bool log_data;               // Flag indicating if data should be logged
} StateMachine;

#endif  // STATE_MACHINE_BASE_H
