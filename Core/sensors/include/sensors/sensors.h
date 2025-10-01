#ifndef SENSORS_H
#define SENSORS_H

#include "sensors/sensors_base.h"

/**
 * @brief Initializes the sensors by setting up necessary configurations and
 * obtaining initial data.
 */
void init_sensors(void);

/**
 * @brief Returns a pointer to the sensor state structure.
 * @return Pointer to the sensor state structure.
 */
const SensorState* get_sensors(void);

/**
 * @brief Updates the sensor states struct by reading the hardware sensors.
 * @param timeout Timeout in microseconds for reading the sensors.
 * @note This function blocks until the reading is complete or the timeout is
 * reached.
 */
void update_sensors(const uint16_t timeout);

/**
 * @brief Clears the sensor readings, resetting them to default values.
 */
void clear_sensors(void);

/**
 * @brief Starts the sensor reading process.
 * @note This function initiates the reading process but does not block.
 * Use sensors_are_reading() to check if the reading is still ongoing.
 */
void start_async_sensors_read(void);

/**
 * @brief Stops the sensor reading process.
 * @note This function stops the reading process initiated by
 * start_async_sensors_read().
 */
void stop_async_sensors_read(void);

/**
 * @brief Checks if the sensors are currently being read.
 * @return true if the sensors are being read, false otherwise.
 * @note This function is meant to enable non-blocking sensor reading.
 */
bool sensors_are_reading(void);

/**
 * @brief Updates the sensor states struct by reading the hardware sensors
 * asynchronously.
 * @return true if the sensors were updated, false if they are still being read.
 */
bool update_sensors_async(void);

#endif  // SENSORS_H
