/**
 * @file matrixOperations.cpp
 * @author Patryk
 * @brief File contains matrix functions
 * @version 0.1
 * @date 12-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <array>
#include <cmath>
#include <iostream>
#include "utils.hpp"

/**
 * @brief Creates a perspective projection matrix.
 *
 * Generates a projection matrix using the specified field of view, aspect ratio,
 * and near/far clipping planes. Used to simulate depth in 3D rendering.
 *
 * @param fov Field of view in degrees.
 * @param aspectRatio Ratio of the viewport width to height.
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 * @return std::array<float, 16> 4x4 perspective projection matrix.
 */
std::array<float, 16> getPerspective(const float fov, const float aspectRatio, const float near, const float far) {
    const float fovRad = toRadians(fov);
    const float t = std::tan(fovRad / 2);

    return {
        1 / (aspectRatio * t), 0, 0, 0,
        0, 1.0f / t, 0, 0,
        0, 0, -((far+near) / (far-near)), -((2 * far * near) / (far-near)),
        0, 0, -1 , 0
    };
}

/**
 * @brief Translates a 4x4 transformation matrix.
 *
 * Applies a translation (movement) to the given matrix by the specified x, y, and z offsets.
 *
 * @param mat The input matrix.
 * @param x Translation distance along the X-axis.
 * @param y Translation distance along the Y-axis.
 * @param z Translation distance along the Z-axis.
 * @return std::array<float, 16> The translated matrix.
 */
std::array<float, 16> translateMatrix(const std::array<float, 16> &mat, const float x, const float y, const float z) {
    std::array<float, 16> mat4 = mat;
    mat4[12] += x;
    mat4[13] += y;
    mat4[14] += z;
    return mat4;
}

/**
 * @brief Multiplies two 4x4 matrices.
 *
 * Performs standard matrix multiplication of two 4x4 matrices (mat1 * mat2).
 *
 * @param mat1 The first matrix (left operand).
 * @param mat2 The second matrix (right operand).
 * @return std::array<float, 16> The resulting matrix.
 */
std::array<float, 16> multiplyMatrix(const std::array<float, 16> &mat1, const std::array<float, 16> &mat2) {
    std::array<float, 16> result = {};

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result[row * 4 + col] = 0;
            for (int k = 0; k < 4; k++) {
                result[row * 4 + col] += mat1[row * 4 + k] * mat2[k * 4 + col];
            }
        }
    }
    return result;
}

/**
 * @brief Scales a 4x4 matrix uniformly.
 *
 * Applies a uniform scaling transformation to the matrix using the given scale factor.
 *
 * @param matrix Input matrix to scale.
 * @param scaleFactor Uniform scaling factor.
 * @return std::array<float, 16> Scaled matrix.
 */
std::array<float, 16> scaleMatrix(const std::array<float, 16> &matrix, const float scaleFactor) {
    auto s = matrix;
    s[0] *= scaleFactor;
    s[5] *= scaleFactor;
    s[10] *= scaleFactor;
    s[12] *= scaleFactor;
    s[13] *= scaleFactor;
    s[14] *= scaleFactor;
    return s;
}

/**
 * @brief Creates a rotation matrix for rotation around the Y-axis.
 *
 * Returns a 4x4 rotation matrix that rotates points around the Y-axis by the specified angle.
 *
 * @param angle Rotation angle in radians.
 * @return std::array<float, 16> Y-axis rotation matrix.
 */
std::array<float, 16> getRotationMatrixY(const float angle) {
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    return {
        c, 0, s, 0,
        0, 1, 0, 0,
       -s, 0, c, 0,
        0, 0, 0, 1
   };
}

/**
 * @brief Returns a 4x4 identity matrix.
 *
 * The identity matrix has 1s on the diagonal and 0s elsewhere.
 * It represents a “no transformation” state.
 *
 * @return std::array<float, 16> Identity matrix.
 */
std::array<float, 16> getIdentityMat4() {
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

/**
 * @brief Prints a 4x4 matrix to the console.
 *
 * @param mat The matrix to print.
 * @param name Optional name to display before the matrix.
 */
void printMatrix(const std::array<float, 16>& mat, const std::string& name) {
    std::cout << name << " = \n";
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            std::cout << mat[row * 4 + col] << "\t";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}
