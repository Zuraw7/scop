/**
 * @file Camera.cpp
 * @author Patryk
 * @brief Camera class implementation
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Camera.hpp"

#include <math.h>

Camera::Camera(const std::array<float, 3> position, const std::array<float, 3> target, const std::array<float, 3> worldUp) : m_camPosition(position), m_targetPos(target), m_worldUp(worldUp), m_yaw(-90.0f), m_pitch(0.0f), m_speed(2.5f), m_fov(45.0f), m_aspectRatio(WIDTH / HEIGHT) {
    updateCameraVectors();
    m_view = lookAt();
    m_projection = getPerspective(m_fov, m_aspectRatio, NEAR, FAR);
}

const std::array<float, 16> &Camera::getCamView() {
    m_view = lookAt();
    return m_view;
}

const std::array<float, 16> &Camera::getCamProjection() const {
    return m_projection;
}

const std::array<float, 3> & Camera::getPosition() const {
    return m_camPosition;
}

/**
 * @brief Updates the camera's orientation based on mouse movement.
 *
 * Adjusts the yaw and pitch angles according to the input deltas, applying a
 * sensitivity factor. The pitch is clamped to [-89, 89] degrees to prevent
 * gimbal lock. After updating angles, the camera's directional vectors are
 * recalculated.
 *
 * @param dx Horizontal mouse movement (affects yaw).
 * @param dy Vertical mouse movement (affects pitch).
 */
void Camera::updateCameraDirection(double dx, double dy) {
    m_yaw += dx * SENSITIVITY;
    m_pitch -= dy * SENSITIVITY;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    else if (m_pitch < -89.0f)
        m_pitch = -89.0f;
    updateCameraVectors();
}

/**
 * @brief Moves the camera in the specified direction.
 *
 * Updates the camera's position by moving it along its directional vectors
 * based on the given direction and time delta. The movement is scaled by
 * the camera's speed.
 *
 * @param dir Direction to move the camera (uses CameraDirection enum).
 * @param deltaTime Time elapsed since the last frame, used to make movement
 *                  frame-rate independent.
 */
void Camera::updateCameraPos(CameraDirection dir, double deltaTime) {
    float velocity = deltaTime * m_speed;

    switch (dir) {
        case CameraDirection::FORWARD:
            m_camPosition = addVec(m_camPosition, multiplyVecByFloat(m_camDirection, velocity));
            break;
        case CameraDirection::BACKWARD:
            m_camPosition = subtractVec(m_camPosition, multiplyVecByFloat(m_camDirection, velocity));
            break;
        case CameraDirection::RIGHT:
            m_camPosition = addVec(m_camPosition, multiplyVecByFloat(m_camRight, velocity));
            break;
        case CameraDirection::LEFT:
            m_camPosition = subtractVec(m_camPosition, multiplyVecByFloat(m_camRight, velocity));
            break;
        case CameraDirection::UP:
            m_camPosition = addVec(m_camPosition, multiplyVecByFloat(m_worldUp, velocity));
            break;
        case CameraDirection::DOWN:
            m_camPosition = subtractVec(m_camPosition, multiplyVecByFloat(m_worldUp, velocity));
            break;
        case CameraDirection::NONE:
            break;
    }
}

/**
 * @brief Adjusts the camera's zoom (field of view).
 *
 * Modifies the camera's field of view (FOV) based on the input delta `dy`.
 * The FOV is clamped between 1.0 and 45.0 degrees to prevent extreme zooming.
 * After updating the FOV, the projection matrix is recalculated.
 *
 * @param dy Change in zoom (positive or negative) from mouse wheel input.
 */
void Camera::updateCameraZoom(double dy) {
    if (m_fov >= 1.0f && m_fov <= 45.0f) {
        m_fov -= dy;
    } else if (m_fov < 1.0f) {
        m_fov = 1.0f;
    } else {
        m_fov = 45.0f;
    }
    m_projection = getPerspective(m_fov, m_aspectRatio, NEAR, FAR);
}

/**
 * @brief Recalculates the camera's directional vectors.
 *
 * Updates the camera's direction, right, and up vectors based on the current
 * yaw and pitch angles. This function ensures that the camera's orientation
 * in 3D space is consistent after changes to yaw, pitch, or any movement.
 */
void Camera::updateCameraVectors() {
    std::array<float, 3> direction{};
    direction[0] = std::cos(toRadians(m_yaw)) * std::cos(toRadians(m_pitch));
    direction[1] = std::sin(toRadians(m_pitch));
    direction[2] = std::sin(toRadians(m_yaw)) * std::cos(toRadians(m_pitch));

    m_camDirection = normalizeVec(direction);
    m_camRight = normalizeVec(crossProdVec(m_camDirection, m_worldUp));
    m_camUp = normalizeVec(crossProdVec( m_camRight, m_camDirection));
}

/**
 * @brief Computes the camera's view matrix using a "look-at" approach.
 *
 * Constructs a 4x4 view matrix that transforms world coordinates into the
 * camera's local coordinate system. It uses the camera's direction,
 * right, and up vectors, along with its position, to correctly orient the
 * scene from the camera's perspective.
 *
 * @return std::array<float, 16> The 4x4 view matrix in column-major order.
 */
std::array<float, 16> Camera::lookAt() {
    std::array<float, 3> forward = normalizeVec(m_camDirection);
    std::array<float, 3> right = normalizeVec(crossProdVec(forward, m_worldUp));
    std::array<float, 3> up = crossProdVec(right, forward);

    std::array<float, 16> result = {
        right[0], up[0], -forward[0], 0,
        right[1], up[1], -forward[1], 0,
        right[2], up[2], -forward[2], 0,
        -dotProdVec(right, m_camPosition), -dotProdVec(up, m_camPosition), dotProdVec(forward, m_camPosition), 1.0f
    };
    return result;
}
