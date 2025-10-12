#include "state_machine/handlers/config_handler.h"

#include "timer/time.h"

static StateMachine sm = {
    .previous_state = STATE_INIT,
    .current_state = STATE_INIT,
    .next_state = STATE_INIT,
    .running_mode = RUNNING_INIT,
    .stop_mode = STOP_MODE_NONE,
    .can_run = false,
    .lap = 0,
    .laps = 0,
    .start_time = 0,
    .stop_time = 0,
    .stop_distance = 0.0f,
    .log_data = false,
};

void reset_state_machine(void) {
    sm.previous_state = STATE_INIT;
    sm.current_state = STATE_INIT;
    sm.next_state = STATE_INIT;
    sm.running_mode = RUNNING_INIT;
    sm.stop_mode = STOP_MODE_NONE;
    sm.can_run = false;
    sm.lap = 0;
    sm.laps = 0;
    sm.start_time = 0;
    sm.stop_time = 0;
    sm.stop_distance = 0.0f;
    sm.log_data = false;
}

void restart_state_machine(void) {
    sm.can_run = false;
    sm.lap = 0;
    sm.start_time = 0;
}

const StateMachine* get_state_machine(void) { return &sm; }

StateMachine* get_state_machine_ptr(void) { return &sm; }

void set_running_mode(const RunningModes mode) { sm.running_mode = mode; }

void set_stop_mode(const StopModes mode) { sm.stop_mode = mode; }

void set_can_run(const bool can_run) { sm.can_run = can_run; }

void set_laps(const uint8_t laps) { sm.laps = laps; }

void add_lap(void) { sm.lap++; }

void set_lap(const uint8_t lap) { sm.lap = lap; }

void set_start_time(void) { sm.start_time = time(); }

void set_stop_time(const uint8_t stop_time) {
    sm.stop_time = stop_time * 1000UL;
}

void set_stop_distance(const float stop_distance) {
    sm.stop_distance = stop_distance;
}

void set_log_data(const bool log_data) { sm.log_data = log_data; }
