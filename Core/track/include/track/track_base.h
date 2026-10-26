#ifndef TRACK_BASE_H
#define TRACK_BASE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @enum LostType
 * @brief Enumeration for different types of line loss.
 */
typedef enum { NONE, LEFT, RIGHT, PITCH } LostType;

/**
 * @struct TrackCounters
 * @brief Structure to hold tracking counters for different line observations.
 */
typedef struct {
    uint16_t line_counter;       // Counts times the line was detected
    uint8_t crossings;           // Crossings detected
    uint8_t curve_markers;       // Curve markers detected
    uint8_t track_markers;       // Track markers detected
    uint8_t right_markers;       // Debounced right sensor activations
    uint8_t lost_left_counter;   // Robot lost to the left of the line
    uint8_t lost_right_counter;  // Robot lost to the right of the line
    uint8_t lost_pitch_counter;  // Robot lost due to the pitch
    float x;                     // X position in cm
    float y;                     // Y position in cm
    float distance;              // Distance traveled in cm
    float heading;               // Heading angle in radians [-π, π]
    float sin_heading;           // Sine of the heading angle
    float cos_heading;           // Cosine of the heading angle
    uint8_t section;             // Current section of the track
    uint8_t laps;                // Current lap count
    float imu_alpha;             // IMU alpha value for heading correction
} TrackCounters;

/**
 * @struct TrackTelemetry
 * @brief Track observations held until a telemetry message consumes them.
 */
typedef struct {
    uint8_t event;      // Last event: 0 none, 1 crossing, 2 curve, 3 marker
    bool right_sensor;  // Right sensor active in any update
} TrackTelemetry;

#endif  // TRACK_BASE_H
