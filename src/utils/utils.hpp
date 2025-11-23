/**
 * @file utils.hpp
 * @author Patryk
 * @brief Header file for all utility functions from utils directory
 * @version 0.1
 * @date 12-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_MATRIXOPERATIONS_HPP
#define SCOP_MATRIXOPERATIONS_HPP

#include <array>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../core/Object.hpp"
#include "../render/Renderer.hpp"

class Object;
class Renderer;

inline double toRadians(const double x) {
    return x * (M_PI / 180);
}

// Window utils
void processInput(GLFWwindow *window, std::unique_ptr<Object> &object, Renderer &renderer, double deltaTIme);

// Matrix operations
std::array<float, 16> getPerspective(const float fov, const float aspectRatio, const float near, const float far);
std::array<float, 16> translateMatrix(const std::array<float, 16> &mat, const float x, const float y, const float z);
std::array<float, 16> scaleMatrix(const std::array<float, 16> &matrix, const float scaleFactor);
std::array<float, 16> multiplyMatrix(const std::array<float, 16> &mat1, const std::array<float, 16> &mat2);
std::array<float, 16> getRotationMatrixY(const float angle);
std::array<float, 16> getIdentityMat4();
void printMatrix(const std::array<float, 16>& mat, const std::string& name);

// Vector operations
std::array<float, 3> subtractVec(const std::array<float, 3> &a, const std::array<float, 3> &b);
std::array<float, 3> addVec(const std::array<float, 3> &a, const std::array<float, 3> &b);
std::array<float, 3> normalizeVec(const std::array<float, 3> &vec);
std::array<float, 3> crossProdVec(const std::array<float, 3> &a, const std::array<float, 3> &b);
float dotProdVec(const std::array<float, 3>& a, const std::array<float, 3>& b);
std::array<float, 3> multiplyVecByFloat(const std::array<float, 3> &vec, float x);

// Callbacks
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#endif //SCOP_MATRIXOPERATIONS_HPP