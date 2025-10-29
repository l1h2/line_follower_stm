#ifndef TRACK_SELECTOR_H
#define TRACK_SELECTOR_H

#include "config.h"

#if SELECTED_TRACK == BASE_SQUARE
#include "track/tracks/base_square.h"
#elif SELECTED_TRACK == INVERSE_SQUARE
#include "track/tracks/inverse_square.h"
#elif SELECTED_TRACK == BASE_TRIANGLE
#include "track/tracks/base_triangle.h"
#elif SELECTED_TRACK == INVERSE_TRIANGLE
#include "track/tracks/inverse_triangle.h"
#elif SELECTED_TRACK == STAR
#include "track/tracks/star.h"
#elif SELECTED_TRACK == HEART
#include "track/tracks/heart.h"
#elif SELECTED_TRACK == PET
#include "track/tracks/pet.h"
#elif SELECTED_TRACK == PET_COMPLEX
#include "track/tracks/pet_complex.h"
#elif SELECTED_TRACK == WAYPOINT_TEST
#include "track/tracks/waypoint_test.h"
#else
#include "track/tracks/base_square.h"
#endif

#endif  // TRACK_SELECTOR_H
