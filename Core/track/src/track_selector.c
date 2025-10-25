#include "track/track_selector.h"

#include "config.h"

#if SELECTED_TRACK == BASE_SQUARE
#include "track/tracks/base_square.h"
#elif SELECTED_TRACK == INVERSE_SQUARE
#include "track/tracks/base_triangle.h"
#elif SELECTED_TRACK == BASE_TRIANGLE
#include "track/tracks/base_triangle.h"
#elif SELECTED_TRACK == INVERSE_TRIANGLE
#include "track/tracks/base_triangle.h"
#else
#include "track/tracks/base_square.h"
#endif

const float* const waypoints_x = track_x;
const float* const waypoints_y = track_y;
const uint16_t waypoint_count = WAYPOINT_COUNT;
