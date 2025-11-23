/**
 * @file inputManagement.cpp
 * @author Patryk
 * @brief File contains functions related with user input
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../core/Camera.hpp"
#include "../core/Object.hpp"
#include "../render/Renderer.hpp"

extern Camera gCamera;

static void changeModes(GLFWwindow *window, Renderer &renderer);
static void moveObject(GLFWwindow *window, std::unique_ptr<Object> &object, double deltaTime);
static void moveCamera(GLFWwindow *window, double deltaTime);

/**
 * @brief Processes all input for the current frame.
 *
 * Handles closing the window, changing rendering modes, moving the object,
 * and moving the camera based on key presses.
 *
 * @param window Pointer to the GLFW window.
 * @param object Reference to the object being controlled.
 * @param renderer Reference to the renderer.
 * @param deltaTime Time elapsed since the last frame (used for frame-rate-independent movement).
 */
void processInput(GLFWwindow *window, std::unique_ptr<Object> &object, Renderer &renderer, double deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    changeModes(window, renderer);
    moveObject(window, object, deltaTime);
    moveCamera(window, deltaTime);
}

/**
 * @brief Handles toggling polygon and color modes.
 *
 * Toggles wireframe/solid polygon mode with 'P' and switches color mode with 'T'.
 * Ensures mode changes only occur once per key press.
 *
 * @param window Pointer to the GLFW window.
 * @param renderer Reference to the renderer.
 */
static void changeModes(GLFWwindow *window, Renderer &renderer) {
    static bool pWasPressed = false;
    static bool tWasPressed = false;

    const bool pIsPressed = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
    const bool tIsPressed = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;

    if (pIsPressed && !pWasPressed)
        renderer.switchPolygonMode();
    pWasPressed = pIsPressed;

    if (tIsPressed && !tWasPressed)
        renderer.toggleColorMode();
    tWasPressed = tIsPressed;
}

/**
 * @brief Handles object movement based on keyboard input.
 *
 * Moves the object along its local axes:
 * - W/S: Y-axis
 * - A/D: X-axis
 * - N/M: Z-axis
 *
 * @param window Pointer to the GLFW window.
 * @param object Reference to the object to move.
 * @param deltaTime Time elapsed since the last frame.
 */
static void moveObject(GLFWwindow *window, std::unique_ptr<Object> &object, double deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        object->moveYaxis(1.0f, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        object->moveYaxis(-1.0f, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        object->moveXaxis(-1.0f, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        object->moveXaxis(1.0f, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        object->moveZaxis(-1.0f, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        object->moveZaxis(1.0f, deltaTime);
}

/**
 * @brief Handles camera movement based on keyboard input.
 *
 * Moves the camera in the 3D scene along its directional vectors:
 * - Arrow keys: Forward/Backward/Left/Right
 * - Space / Left Shift: Up/Down
 *
 * @param window Pointer to the GLFW window.
 * @param deltaTime Time elapsed since the last frame.
 */
static void moveCamera(GLFWwindow *window, double deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::DOWN, deltaTime);
    }
}
