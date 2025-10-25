#include "math/math.h"

#include <stdint.h>

#define MATH_PI_2 (2.0f * MATH_PI)
#define RAD_TO_DEG (180.0f / MATH_PI)
#define DEG_TO_RAD (MATH_PI / 180.0f)
#define ONE_OVER_SIX (1.0f / 6.0f)

float fast_inv_sqrtf(const float x) {
    uint32_t i;

    const float x2 = x * 0.5f;
    float y = x;

    i = *(uint32_t*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;

    y = y * (1.5f - (x2 * y * y));
    y = y * (1.5f - (x2 * y * y));

    return y;
}

void sincos_poly_truncation(const float a, float* const s, float* const c) {
    const float a2 = a * a;
    *s = a * (1.0f - a2 * ONE_OVER_SIX);
    *c = 1.0f - 0.5f * a2;
}

void normalize_unit_vector(float* const cos, float* const sin) {
    const float n2 = (*cos) * (*cos) + (*sin) * (*sin);
    const float inv_len = 0.5f * (3.0f - n2);
    *cos *= inv_len;
    *sin *= inv_len;
}

void normalize_angle(float* const angle) {
    while (*angle > MATH_PI) {
        *angle -= MATH_PI_2;
    }
    while (*angle < -MATH_PI) {
        *angle += MATH_PI_2;
    }
}

float rad_to_deg(const float angle) { return (angle * RAD_TO_DEG); }

float deg_to_rad(const float angle) { return (angle * DEG_TO_RAD); }
