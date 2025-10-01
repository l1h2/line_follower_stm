#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#include <stdbool.h>
#include <stdint.h>

#define TOTAL_CENTRAL_SENSORS 8  // Total number of central sensors
#define TOTAL_SIDE_SENSORS 2     // Total number of side sensors

/**
 * @brief Start the sensor reading process.
 */
void start_read(void);

/**
 * @brief Stop the sensor reading process.
 */
void stop_read(void);

/**
 * @brief Read the central sensors with a specified timeout.
 *
 * @param timeout Timeout in microseconds for reading the sensors.
 * @return uint8_t A byte representing the state of the central sensors.
 * @note This function blocks until the reading is complete or the timeout is
 * reached.
 */
uint8_t read_central_sensors(const uint16_t timeout);

/**
 * @brief Get the raw values from the central sensors with a specified timeout.
 *
 * @return uint16_t* Pointer to an array of raw sensor values.
 * @warning This function doesn't perform a read itself; it should be called
 * after read_central_sensors() to get the latest values.
 * @note This function blocks until the reading is complete or the timeout is
 * reached.
 */
uint16_t* get_central_sensor_values(void);

/**
 * @brief Checks if the central sensors are currently being read.
 *
 * @param timeout Timeout in microseconds for reading the sensors.
 * @return true if the sensors are being read, false otherwise.
 * @note This function is meant to enable non-blocking sensor reading.
 */
bool central_sensor_is_reading(const uint16_t timeout);

/**
 * @brief Asynchronously read the central sensors.
 *
 * @return uint8_t A byte representing the state of the central sensors.
 * @note This function does not block and returns the current sensor states.
 * @note This function should be called after start_read() and before
 * stop_read().
 * @note This function should be used in combination with
 * central_sensor_is_reading() to check if the reading process is still ongoing.
 */
uint8_t read_central_sensors_async(void);

/**
 * @brief Get the current values of the side sensors.
 *
 * @return bool* Pointer to an array of boolean values representing the state
 * of the side sensors.
 */
bool* get_side_sensor_values(void);

#endif  // IR_SENSORS_H
