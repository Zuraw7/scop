/**
 * @file VertexArray.cpp
 * @author Patryk
 * @brief VertexArray (VAO) class implementation
 * @version 0.1
 * @date 10-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "VertexArray.hpp"

/**
 * @brief Creates a new Vertex Array Object (VAO) and binds it.
 *
 * Generates an OpenGL VAO, stores its ID, and binds it to the current context.
 */
VertexArray::VertexArray() : m_id(0) {
    glGenVertexArrays(1, &m_id);
    glBindVertexArray(m_id);
}

VertexArray::VertexArray(VertexArray &&other) noexcept
    : m_id(other.m_id) {
    other.m_id = 0;
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
    if (this == &other)
        return *this;
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

VertexArray::~VertexArray() {
    if (m_id)
        glDeleteVertexArrays(1, &m_id);
}

/**
 * @brief Binds the VAO to the current OpenGL context.
 *
 * After calling this function, the VAO is active and vertex buffers/attributes
 * associated with it will be used for rendering.
 */
void VertexArray::bind() const {
    glBindVertexArray(m_id);
}

/**
 * @brief Unbinds the VAO from the current OpenGL context.
 *
 * Ensures that no VAO is bound, preventing accidental use of this VAO in subsequent draw calls.
 */
void VertexArray::unbind() const {
    glBindVertexArray(0);
}
