#ifndef TRACK_SELECTOR_H
#define TRACK_SELECTOR_H

#include "config.h"

#if SELECTED_TRACK == BASE_SQUARE
#include "track/tracks/base_square.h"
#define TRACK_NAME "BASE_SQUARE"
#elif SELECTED_TRACK == INVERSE_SQUARE
#include "track/tracks/inverse_square.h"
#define TRACK_NAME "INVERSE_SQUARE"
#elif SELECTED_TRACK == BASE_TRIANGLE
#include "track/tracks/base_triangle.h"
#define TRACK_NAME "BASE_TRIANGLE"
#elif SELECTED_TRACK == INVERSE_TRIANGLE
#include "track/tracks/inverse_triangle.h"
#define TRACK_NAME "INVERSE_TRIANGLE"
#elif SELECTED_TRACK == STAR
#include "track/tracks/star.h"
#define TRACK_NAME "STAR"
#elif SELECTED_TRACK == HEART
#include "track/tracks/heart.h"
#define TRACK_NAME "HEART"
#elif SELECTED_TRACK == PET
#include "track/tracks/pet.h"
#define TRACK_NAME "PET"
#elif SELECTED_TRACK == PET_COMPLEX
#include "track/tracks/pet_complex.h"
#define TRACK_NAME "PET_COMPLEX"
#elif SELECTED_TRACK == WAYPOINT_TEST
#include "track/tracks/waypoint_test.h"
#define TRACK_NAME "WAYPOINT_TEST"
#elif SELECTED_TRACK == PET_TEST
#include "track/tracks/pet_test.h"
#define TRACK_NAME "PET_TEST"
#elif SELECTED_TRACK == PET_10_LAPS
#include "track/tracks/pet_10_laps.h"
#define TRACK_NAME "PET_10_LAPS"
#elif SELECTED_TRACK == PET_SMALL
#include "track/tracks/pet_small.h"
#define TRACK_NAME "PET_SMALL"
#else
#error "SELECTED_TRACK has no matching track header in track_selector.h"
#endif

#endif  // TRACK_SELECTOR_H
