#include "pid/errors/errors.h"

#include <stdlib.h>

#include "hal/ir_sensors.h"
#include "pid/errors/speed_errors.h"
#include "sensors/sensors.h"

#define ERROR_WEIGHT 2
#define AVG_ERROR ((ERROR_WEIGHT * (TOTAL_CENTRAL_SENSORS - 1)) / 2)

#define MAX_ERROR (TOTAL_CENTRAL_SENSORS - 1) * ERROR_WEIGHT - AVG_ERROR
#define MIN_ERROR (-MAX_ERROR)
#define MAX_ERROR_SUM 1000
#define MIN_ERROR_SUM -MAX_ERROR_SUM

static ErrorStruct errors = {
    .error = 0,
    .last_error = 0,
    .delta_error = 0,
    .error_sum = 0,
    .feedforward = 0,
    .max_error = MAX_ERROR,
    .min_error = MIN_ERROR,
    .speed_errors = NULL,
    .sensors = NULL,
};

static bool is_updating_sensors = false;

static inline void update_error(void) {
    const uint8_t central_sensors_state =
        errors.sensors->ir_sensors->central_sensors_state;
    if (!central_sensors_state) return;

    uint8_t count = 0;
    int8_t new_error = 0;

    for (uint8_t i = 0; i <= TOTAL_CENTRAL_SENSORS - 1; i++) {
        if (!(central_sensors_state & (1 << i))) continue;

        new_error += i;
        count++;
    }

    errors.error = new_error * ERROR_WEIGHT / count - AVG_ERROR;
}

static inline void update_error_sum(void) {
    errors.error_sum += errors.error;

    if (errors.error_sum > MAX_ERROR_SUM) {
        errors.error_sum = MAX_ERROR_SUM;
    } else if (errors.error_sum < MIN_ERROR_SUM) {
        errors.error_sum = MIN_ERROR_SUM;
    }
}

static inline void update_delta_error(void) {
    errors.delta_error = errors.error - errors.last_error;
}

static inline void update_last_error(void) { errors.last_error = errors.error; }

static void update_feedforward(void) {
    // TODO: Implement feedforward based on sensor data
    errors.feedforward = 0;
}

static inline bool check_sensor_update(const bool read_encoder) {
    if (!is_updating_sensors) {
        start_async_sensors_read();
        is_updating_sensors = true;
        return false;
    }

    if (!update_sensors_async(read_encoder)) return false;

    is_updating_sensors = false;
    stop_async_sensors_read();
    return true;
}

const ErrorStruct* init_errors(const SensorState* const sensors) {
    errors.sensors = sensors;
    errors.speed_errors = init_speed_errors(&errors);
    return &errors;
}

const ErrorStruct* get_errors(void) { return &errors; }

void update_errors(const uint16_t timeout, const bool read_encoder) {
    update_sensors(timeout, read_encoder);

    update_error();
    update_error_sum();
    update_delta_error();
    update_last_error();
    update_feedforward();
}

bool update_errors_async(const bool read_encoder) {
    if (!check_sensor_update(read_encoder)) return false;

    update_error();
    update_error_sum();
    update_delta_error();
    update_last_error();
    update_feedforward();

    return true;
}

void clear_errors(void) {
    errors.error = 0;
    errors.last_error = 0;
    errors.delta_error = 0;
    errors.error_sum = 0;
    clear_sensors();
}
