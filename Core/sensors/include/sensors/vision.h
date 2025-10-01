#ifndef VISION_H
#define VISION_H

#include <stdbool.h>
#include <stdint.h>

#include "sensors/sensors_base.h"

#define SENSOR_READ_TIMEOUT_US 300  // Timeout for sensor reading

/**
 * @brief Returns a pointer to the sensor state structure.
 * @return Pointer to the sensor state structure.
 */
const IrSensorData* get_ir_sensors(void);

/**
 * @brief Updates the sensor states struct by reading the hardware sensors.
 * @param timeout Timeout in microseconds for reading the sensors.
 * @note This function blocks until the reading is complete or the timeout is
 * reached.
 */
void update_ir_sensors(const uint16_t timeout);

/**
 * @brief Clears the sensor states struct.
 */
void clear_ir_sensors(void);

/**
 * @brief Starts the sensor reading process.
 * @note This function initiates the reading process but does not block.
 * Use ir_sensors_are_reading() to check if the reading is still ongoing.
 */
void start_ir_sensors_read(void);

/**
 * @brief Stops the sensor reading process.
 * @note This function stops the reading process initiated by
 * start_ir_sensors_read().
 */
void stop_ir_sensors_read(void);

/**
 * @brief Checks if the sensors are currently being read.
 * @return true if the sensors are being read, false otherwise.
 * @note This function is meant to enable non-blocking sensor reading.
 */
bool ir_sensors_are_reading(void);

/**
 * @brief Updates the sensor states struct by reading the hardware sensors
 * asynchronously.
 * @return true if the sensors were updated, false if they are still being read.
 */
bool update_ir_sensors_async(void);

/**
 * @brief Gets the raw times from the central sensors.
 * @return Pointer to an array of raw sensor values.
 * @note This function blocks until the reading is complete or the timeout is
 * reached.
 */
uint16_t* get_central_ir_sensor_times(void);

#endif  // VISION_H
