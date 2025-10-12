#include "state_machine/running_modes/running_base.h"

#include <stdlib.h>

#include "logger/logger.h"
#include "serial/serial_in.h"
#include "state_machine/handlers/config_handler.h"
#include "timer/time.h"
#include "turbine/turbine.h"

static const TrackCounters* track = NULL;
static const StateMachine* sm = NULL;

static inline void check_stop_time(void) {
    if (!time_elapsed(sm->start_time, sm->stop_time)) return;

    set_can_run(false);
}

static inline void check_stop_laps(void) {
    set_lap(track->laps);

    if (sm->lap >= sm->laps) set_can_run(false);
}

static inline void check_stop_distance(void) {
    if (track->distance >= sm->stop_distance) set_can_run(false);
}

void init_running_modes(const TrackCounters* const track_counters) {
    track = track_counters;
    sm = get_state_machine();
}

void check_stop(const bool track_updated) {
    switch (sm->stop_mode) {
        case STOP_MODE_NONE:
            return;
        case STOP_MODE_TIME:
            check_stop_time();
            break;
        case STOP_MODE_LAPS:
            if (!track_updated) return;
            check_stop_laps();
            break;
        case STOP_MODE_DISTANCE:
            check_stop_distance();
            break;
    }
}

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
