#include "state_machine/running_modes/running_base.h"

#include "logger/logger.h"
#include "serial/serial_in.h"
#include "turbine/turbine.h"

void start_turbine_if_needed(void) {
    if (!turbine_can_run()) return;

    debug_print("Starting turbine before running");

    while (!turn_on_turbine_async(TURBINE_DEFAULT_ON_TIME)) {
        process_serial_messages();
    }

    debug_print("Turbine finished starting");
}

void stop_turbine_if_needed(void) {
    if (!turbine_can_run()) return;

    debug_print("Stopping turbine");
    turn_off_turbine();
}
