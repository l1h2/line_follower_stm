#include "track/tracks/inverse_square.h"

#include "config.h"

#if SELECTED_TRACK == INVERSE_SQUARE
const float track_x[WAYPOINT_COUNT] = {0, 50, 50, 0};
const float track_y[WAYPOINT_COUNT] = {0, 0, -50, -50};
#endif
