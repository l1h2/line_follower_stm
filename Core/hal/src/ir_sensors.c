#include "hal/ir_sensors.h"

#include <string.h>

#include "main.h"
#include "timer/time.h"

typedef struct {
    GPIO_TypeDef* port;
    uint32_t pin;
} SensorPin;

static const SensorPin central_sensors[] = {
    {SENSOR_IR_0_GPIO_Port, SENSOR_IR_0_Pin},
    {SENSOR_IR_1_GPIO_Port, SENSOR_IR_1_Pin},
    {SENSOR_IR_2_GPIO_Port, SENSOR_IR_2_Pin},
    {SENSOR_IR_3_GPIO_Port, SENSOR_IR_3_Pin},
    {SENSOR_IR_4_GPIO_Port, SENSOR_IR_4_Pin},
    {SENSOR_IR_5_GPIO_Port, SENSOR_IR_5_Pin},
    {SENSOR_IR_6_GPIO_Port, SENSOR_IR_6_Pin},
    {SENSOR_IR_7_GPIO_Port, SENSOR_IR_7_Pin},
};

static const SensorPin side_sensors[] = {
    {SENSOR_IR_LEFT_GPIO_Port, SENSOR_IR_LEFT_Pin},
    {SENSOR_IR_RIGHT_GPIO_Port, SENSOR_IR_RIGHT_Pin},
};

static uint32_t start_time = 0;
static uint16_t central_sensor_values[TOTAL_CENTRAL_SENSORS] = {0};
static uint8_t central_sensor_byte = 0;
static bool central_sensor_reading_started = false;

static inline void set_sensors_output_high(void) {
    for (uint8_t i = 0; i < TOTAL_CENTRAL_SENSORS; i++) {
        LL_GPIO_SetPinMode(central_sensors[i].port, central_sensors[i].pin,
                           LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetOutputPin(central_sensors[i].port, central_sensors[i].pin);
    }
}

static inline void set_sensors_input(void) {
    for (uint8_t i = 0; i < TOTAL_CENTRAL_SENSORS; i++) {
        LL_GPIO_SetPinMode(central_sensors[i].port, central_sensors[i].pin,
                           LL_GPIO_MODE_INPUT);
    }
}

static void sensors_read_raw(const uint16_t timeout) {
    uint8_t sensors_read = 0;
    uint32_t read_interval = time_us() - start_time;

    while (read_interval < timeout && sensors_read < TOTAL_CENTRAL_SENSORS) {
        for (uint8_t i = 0; i < TOTAL_CENTRAL_SENSORS; i++) {
            if (central_sensor_values[i] == 0 &&
                !LL_GPIO_IsInputPinSet(central_sensors[i].port,
                                       central_sensors[i].pin)) {
                central_sensor_values[i] = read_interval;
                sensors_read++;
            }
        }

        read_interval = time_us() - start_time;
    }

    for (uint8_t i = 0; i < TOTAL_CENTRAL_SENSORS; i++) {
        if (central_sensor_values[i] == 0) {
            central_sensor_values[i] = timeout;
        } else {
            central_sensor_byte |= (1 << i);
        }
    }
}

void start_read(void) {
    LL_GPIO_SetOutputPin(SENSOR_IR_INPUT_GPIO_Port, SENSOR_IR_INPUT_Pin);
    set_sensors_input();
    memset(central_sensor_values, 0, sizeof(central_sensor_values));
    central_sensor_byte = 0;
    start_time = time_us();
    central_sensor_reading_started = true;
}

void stop_read(void) {
    LL_GPIO_ResetOutputPin(SENSOR_IR_INPUT_GPIO_Port, SENSOR_IR_INPUT_Pin);
    set_sensors_output_high();
    central_sensor_reading_started = false;
}

uint8_t read_central_sensors(const uint16_t timeout) {
    start_read();
    sensors_read_raw(timeout);
    stop_read();

    return central_sensor_byte;
}

uint16_t* get_central_sensor_values(void) { return central_sensor_values; }

bool central_sensor_is_reading(const uint16_t timeout) {
    return (central_sensor_reading_started &&
            !time_elapsed_us(start_time, timeout));
}

uint8_t read_central_sensors_async(void) {
    for (uint8_t i = 0; i < TOTAL_CENTRAL_SENSORS; i++) {
        if (!LL_GPIO_IsInputPinSet(central_sensors[i].port,
                                   central_sensors[i].pin)) {
            central_sensor_byte |= (1 << i);
        }
    }

    return central_sensor_byte;
}

bool* get_side_sensor_values(void) {
    static bool side_sensor_values[TOTAL_SIDE_SENSORS] = {0};

    for (uint8_t i = 0; i < TOTAL_SIDE_SENSORS; i++) {
        side_sensor_values[i] =
            !LL_GPIO_IsInputPinSet(side_sensors[i].port, side_sensors[i].pin);
    }

    return side_sensor_values;
}
