/**
 * @file VertexBuffer.cpp
 * @author Patryk
 * @brief VertexBuffer (VBO) class implementation
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "VertexBuffer.hpp"
#include "../core/Object.hpp"

/**
 * @brief Creates a VertexBuffer from a raw array of floats.
 *
 * This constructor generates an OpenGL Vertex Buffer Object (VBO),
 * binds it to the current context, and fills it with the provided
 * vertex data from a raw array of floats.
 *
 * @param vertices Pointer to an array of floats representing vertex data.
 * @param size Number of floats in the array.
 */
VertexBuffer::VertexBuffer(const float *vertices, const size_t size) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
}

/**
 * @brief Creates a VertexBuffer from a std::vector of floats.
 *
 * This constructor generates an OpenGL Vertex Buffer Object (VBO),
 * binds it to the current context, and fills it with the vertex data
 * contained in the vector.
 *
 * @param vertices Vector containing floats representing vertex data.
 */
VertexBuffer::VertexBuffer(const std::vector<float> &vertices) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

/**
 * @brief Creates a VertexBuffer from a std::vector of Vertex structs.
 *
 * This constructor generates an OpenGL Vertex Buffer Object (VBO),
 * binds it to the current context, and fills it with the vertex data
 * contained in the vector of Vertex structures.
 *
 * @param vertices Vector containing Vertex structs representing the object's vertex data.
 */
VertexBuffer::VertexBuffer(const std::vector<Vertex> &vertices) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
    : m_id(other.m_id) {
    other.m_id = 0;
}

VertexBuffer::~VertexBuffer() {
    if (m_id)
        glDeleteBuffers(1, &m_id);
}

VertexBuffer & VertexBuffer::operator=(VertexBuffer &&other) noexcept {
    if (this == &other)
        return *this;
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

/**
 * @brief Binds the Vertex Buffer Object (VBO) to the current OpenGL context.
 *
 * After calling this function, the VBO becomes active, and any subsequent
 * vertex attribute or draw calls will use this buffer.
 */
void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

/**
 * @brief Unbinds the Vertex Buffer Object (VBO) from the current OpenGL context.
 *
 * This ensures that no VBO is currently bound, preventing accidental usage
 * in subsequent vertex operations or draw calls.
 */
void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
