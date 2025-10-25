#ifndef PURE_PURSUIT_MATH_H
#define PURE_PURSUIT_MATH_H

#define MATH_PI 3.14159265358979323846f

/**
 * @brief Fast inverse square root approximation
 * @param x Input value
 * @return Approximated inverse square root of x
 */
float fast_inv_sqrtf(const float x);

/**
 * @brief Compute sine and cosine using 0-centered Taylor polynomial truncation
 * @param a Angle in radians
 * @param s Pointer to store sine result
 * @param c Pointer to store cosine result
 * @note Accurate for angles smaller than 0.3 radians (17°)
 */
void sincos_poly_truncation(const float a, float* const s, float* const c);

/**
 * @brief Normalize a 2D unit vector with 1 Newton–Raphson iteration
 * @param cos Pointer to cosine component
 * @param sin Pointer to sine component
 */
void normalize_unit_vector(float* const cos, float* const sin);

/**
 * @brief Normalize an angle to the range [-π, π]
 * @param angle Pointer to the angle in radians
 */
void normalize_angle(float* const angle);

/**
 * @brief Convert radians to degrees
 * @param angle Angle in radians
 * @return Angle in degrees
 */
float rad_to_deg(const float angle);

/**
 * @brief Convert degrees to radians
 * @param angle Angle in degrees
 * @return Angle in radians
 */
float deg_to_rad(const float angle);

#endif  // PURE_PURSUIT_MATH_H
