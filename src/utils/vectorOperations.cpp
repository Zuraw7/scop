/**
 * @file vectorOperations.cpp
 * @author Patryk
 * @brief File contains vector functions
 * @version 0.1
 * @date 12-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <array>
#include <cmath>

/**
 * @brief Subtracts one 3D vector from another.
 *
 * @param a The first vector.
 * @param b The second vector to subtract from the first.
 * @return A new vector representing the difference (a - b).
 */
std::array<float, 3> subtractVec(const std::array<float, 3> &a, const std::array<float, 3> &b) {
    return { a[0] - b[0], a[1] - b[1], a[2] - b[2] };
}

/**
 * @brief Adds two 3D vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return A new vector representing the sum (a + b).
 */
std::array<float, 3> addVec(const std::array<float, 3> &a, const std::array<float, 3> &b) {
    return { a[0] + b[0], a[1] + b[1], a[2] + b[2] };
}

/**
 * @brief Normalizes a 3D vector.
 *
 * Converts the vector into a unit vector (length = 1) while preserving its direction.
 * If the input vector has zero length, a zero vector is returned to avoid division by zero.
 *
 * @param vec The input vector.
 * @return A normalized version of the input vector.
 */
std::array<float, 3> normalizeVec(const std::array<float, 3> &vec) {
    const float length = std::sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));
    if (length == 0) return { 0.0f, 0.0f, 0.0f };
    return { vec[0] / length, vec[1] / length, vec[2] / length };
}

/**
 * @brief Computes the cross product of two 3D vectors.
 *
 * Returns a vector that is perpendicular to both input vectors.
 * The result follows the right-hand rule.
 *
 * @param a The first input vector.
 * @param b The second input vector.
 * @return The cross product of vector a and b.
 */
std::array<float, 3> crossProdVec(const std::array<float, 3> &a, const std::array<float, 3> &b) {
    const float ax = a[0], ay = a[1], az = a[2];
    const float bx = b[0], by = b[1], bz = b[2];
    return {
        ((ay * bz) - (az * by)),
        ((az * bx) - (ax * bz)),
        ((ax * by) - (bx * ay))
    };
}

/**
 * @brief Computes the dot product (scalar product) of two 3D vectors.
 *
 * Measures how aligned two vectors are with each other.
 * A result of 1 means they point in the same direction,
 * 0 means they are perpendicular, and -1 means they are opposite.
 *
 * @param a The first input vector.
 * @param b The second input vector.
 * @return The dot product value (a * b).
 */
float dotProdVec(const std::array<float, 3>& a, const std::array<float, 3>& b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

/**
 * @brief Multiplies a 3D vector by a scalar value.
 *
 * Scales the vector uniformly in all directions by the given scalar.
 *
 * @param vec The input vector.
 * @param x The scalar value to multiply by.
 * @return A new scaled vector.
 */
std::array<float, 3> multiplyVecByFloat(const std::array<float, 3> &vec, float x) {
    return { vec[0] * x, vec[1] * x, vec[2] * x };
}
