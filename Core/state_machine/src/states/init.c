#include "state_machine/states/init.h"

#include "logger/logger.h"
#include "motors/motors.h"
#include "pid/pid.h"
#include "pure_pursuit/pure_pursuit.h"
#include "sensors/sensors.h"
#include "serial/serial_base.h"
#include "serial/serial_out.h"
#include "state_machine/handlers/state_handler.h"
#include "state_machine/running_modes/running_base.h"
#include "timer/time.h"
#include "track/track.h"

void handle_init(const StateMachine* const sm) {
    debug_print("INIT State: Initializing state machine");

    init_serial();
    init_timer();
    init_motors();

    const SensorState* const sensors = init_sensors();
    const PidStruct* const pid = init_pid(sensors);
    const TrackCounters* const track_counters = init_track(pid->errors);
    const PurePursuit* const pp = init_pure_pursuit(track_counters, pid);

    init_running_modes(track_counters);
    init_serial_out(sm, sensors, pid, track_counters, pp);

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
