#include "state_machine/states/init.h"

// HAL includes
#include "hal/pwm.h"
#include "hal/spi.h"
#include "hal/timer.h"
#include "hal/usart.h"

// Other modules
#include "logger/logger.h"
#include "pid/pid.h"
#include "sensors/sensors.h"
#include "state_machine/handlers/state_handler.h"

void handle_init(void) {
    debug_print("INIT State: Initializing state machine");

    // HAL initialization
    init_system_timer();
    init_pwm();
    init_usart();
    init_spi();

    // Other modules initialization
    init_sensors();
    init_pid();

    request_next_state(STATE_IDLE);
}

static void handle_init_to_idle(void) {
    debug_print("Transitioning from INIT to IDLE");
}

bool handle_init_transitions(const RobotStates next_state) {
    switch (next_state) {
        case STATE_IDLE:
            handle_init_to_idle();
            break;
        default:
            return false;
    }

    return true;
}
