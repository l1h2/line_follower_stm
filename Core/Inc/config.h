#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

#define DEBUG_MODE  // Comment to disable debug mode

#define BASE_SQUARE 0       // 0.5 meter square track (default)
#define INVERSE_SQUARE 1    // 0.5 meter inverse square track
#define BASE_TRIANGLE 2     // 0.5 meter triangle track
#define INVERSE_TRIANGLE 3  // 0.5 meter inverse triangle track
#define STAR 4              // Star-shaped track
#define HEART 5             // Heart-shaped track
#define PET 6               // PET track
#define PET_COMPLEX 7       // PET track with all waypoints
#define WAYPOINT_TEST 8     // Test track for generated waypoints

// Default selected track (override with -DSELECTED_TRACK=base_triangle)
#define SELECTED_TRACK PET_COMPLEX

#endif  // LINE_FOLLOWER_H
