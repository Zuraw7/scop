/**
 * @file IndexBuffer.cpp
 * @author Patryk
 * @brief IndexBuffer (IBO/EBO) class implementation
 * @version 0.1
 * @date 10-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "IndexBuffer.hpp"

/**
 * @brief Creates an IndexBuffer from a raw array of indices.
 *
 * This constructor generates an OpenGL element array buffer (EBO),
 * binds it, and fills it with the provided indices.
 *
 * @param indices List of object's indexes
 * @param size Index count
 */
IndexBuffer::IndexBuffer(const unsigned int *indices, const size_t size) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

/**
 * @brief Creates an IndexBuffer from a std::vector of indices.
 *
 * This constructor generates an OpenGL element array buffer (EBO),
 * binds it, and fills it with the indices contained in the vector.
 *
 * @param indices Vector of unsigned integers representing the object's indices.
 */
IndexBuffer::IndexBuffer(const std::vector<unsigned int> &indices) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept
    : m_id(other.m_id) {
    other.m_id = 0;
}

IndexBuffer::~IndexBuffer() {
    if (m_id)
        glDeleteBuffers(1, &m_id);
}

IndexBuffer & IndexBuffer::operator=(IndexBuffer &&other) noexcept {
    if (this == &other)
        return *this;
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

/**
 * @brief Binds IndexBuffer to the current OpenGL context
 */
void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

/**
 * @brief Unbinds IndexBuffer from the current OpenGL context
 */
void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}