/**
 * @file callbacks.cpp
 * @author Patryk
 * @brief File contains callback functions
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../core/Camera.hpp"

extern Camera gCamera;

/**
 * @brief Callback function for framebuffer size changes.
 *
 * Updates the OpenGL viewport to match the new window dimensions.
 *
 * @param window The GLFW window that triggered the callback.
 * @param width New width of the framebuffer.
 * @param height New height of the framebuffer.
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

bool firstMouse = true;
double lastX = 0.0f;
double lastY = 0.0f;

/**
 * @brief Callback function for mouse movement.
 *
 * Tracks the cursor position and updates the camera's direction
 * based on mouse movement. Handles first-mouse initialization
 * to prevent large jumps on the first frame.
 *
 * @param window The GLFW window that triggered the callback.
 * @param xpos Current X position of the cursor.
 * @param ypos Current Y position of the cursor.
 */
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    (void) window;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double offsetX = xpos - lastX;
    double offsetY = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    if (offsetX != 0 || offsetY != 0)
        gCamera.updateCameraDirection(offsetX, offsetY);
}

/**
 * @brief Callback function for scroll input.
 *
 * Updates the camera's zoom level based on vertical scroll offset.
 *
 * @param window The GLFW window that triggered the callback.
 * @param xoffset Horizontal scroll offset (unused).
 * @param yoffset Vertical scroll offset.
 */
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    (void) window;
    (void) xoffset;
    gCamera.updateCameraZoom(yoffset);
}
