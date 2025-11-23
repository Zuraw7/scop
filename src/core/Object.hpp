/**
 * @file Object.hpp
 * @author Patryk
 * @brief Object class declaration
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_OBJECT_HPP
#define SCOP_OBJECT_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>

#include "../textures/Material.hpp"
#include "../textures/Texture2D.hpp"
#include "../utils/utils.hpp"
#include "../graphics/VertexArray.hpp"
#include "../graphics/VertexBuffer.hpp"
#include "../graphics/IndexBuffer.hpp"

#define MOVE_SPEED 2.0

/**
 * @brief Represents a single vertex with position and texture coordinates.
 */
struct Vertex {
    std::array<float, 3> position;
    std::array<float, 2> uv;
    std::array<float, 3> normal;
};

/**
 * @brief Represents a 3D object loaded from an .obj file.
 *
 * Object contains vertex and index data, transformation matrices,
 * a texture, and OpenGL buffers (VAO, VBO, IBO).
 */
class Object {
public:
    Object() = default;
    static std::unique_ptr<Object> create(const std::string &objFilePath);
    Object(const Object&) = delete;
    Object(Object &&other) noexcept;
    ~Object() = default;

    Object &operator=(const Object&) = delete;
    Object &operator=(Object &&other) noexcept;

    void bind() const;
    void unbind() const;
    void updateRotationMatrixY(const float angle);
    void moveXaxis(const float direction, const double deltaTime);
    void moveYaxis(const float direction, const double deltaTime);
    void moveZaxis(const float direction, const double deltaTime);

    const VertexArray &getVAO() const;
    std::array<float, 3> getCenter() const;
    std::vector<unsigned int> getIndices() const;
    std::array<float, 16> getMatrix();
    std::string getTexture2DPath() const;
    const std::array<float, 3> getPosition() const;
    const std::unique_ptr<Material> &getMaterial();

    void setTexture2D(const std::shared_ptr<Texture2D> &texture);

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::array<float, 3> m_center;

    std::array<float, 16> m_translationMatrix;
    std::array<float, 16> m_rotationMatrix;
    std::array<float, 16> m_matrix;

    std::shared_ptr<Texture2D> m_texture2D;

    float m_scaleFactor;

    VertexArray m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO = nullptr;
    std::unique_ptr<IndexBuffer> m_IBO = nullptr;

    std::unique_ptr<Material> m_material = nullptr;

    int parseFile(const std::string &filePath);
    void initBuffers();
    std::array<float, 3> calculateCenter() const;
    float calculateScale() const;
    void calculateUV_XY();
    void calculateUV_ZY();
    void computeNormals();
};


#endif //SCOP_OBJECT_HPP