/**
 * @file Camera.hpp
 * @author Patryk
 * @brief Camera class declaration
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_CAMERA_HPP
#define SCOP_CAMERA_HPP

#include <iostream>
#include <array>

#include "../utils/utils.hpp"

#define NEAR 1.0f
#define FAR 10.0f
#define WIDTH 1980
#define HEIGHT 1080
#define SENSITIVITY 0.1f

/**
 * @brief Possible movement directions for the Camera.
 */
enum CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

/**
 * @brief Represents a 3D camera with position, orientation, and projection.
 *
 * The Camera class provides functions for moving, rotating, and zooming a camera
 * in 3D space. It maintains view and projection matrices and can be used to
 * control the viewpoint in OpenGL rendering.
 */
class Camera {
public:
    Camera() = delete;
    Camera(const std::array<float, 3> position, const std::array<float, 3> target, const std::array<float, 3> worldUp);
    Camera(Camera &) = delete;
    Camera(Camera &&) = delete;
    ~Camera() = default;

    Camera &operator=(const Camera&) = delete;
    Camera &operator=(const Camera&&) = delete;

    const std::array<float, 16> &getCamView();
    const std::array<float, 16> &getCamProjection() const;
    const std::array<float, 3> &getPosition() const;

    void updateCameraDirection(double dx, double dy);
    void updateCameraPos(CameraDirection dir, double deltaTime);
    void updateCameraZoom(double dy);
    void updateCameraVectors();

private:
    // Vectors
    std::array<float, 3> m_camPosition;
    std::array<float, 3> m_targetPos;
    std::array<float, 3> m_worldUp;

    std::array<float, 3> m_camDirection;
    std::array<float, 3> m_camRight;
    std::array<float, 3> m_camUp;

    // Matrices
    std::array<float, 16> m_view;
    std::array<float, 16> m_projection;

    float m_yaw;
    float m_pitch;
    float m_speed;
    float m_fov;
    float m_aspectRatio;

    std::array<float, 16> lookAt();
};

#endif //SCOP_CAMERA_HPP
