/**
 * @file IndexBuffer.hpp
 * @author Patryk
 * @brief  IndexBuffer (IBO/EBO) class declaration
 * @version 0.1
 * @date 10-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_INDEXBUFFER_HPP
#define SCOP_INDEXBUFFER_HPP
#include <vector>
#include <GL/glew.h>

/**
 * @brief Wraps an OpenGL IndexBuffer (element array buffer).
 *
 * The IndexBuffer class is responsible for creating, binding, and unbinding
 * an element array buffer in GPU memory, storing the indices used for
 * indexed rendering.
 */
class IndexBuffer {
public:
    IndexBuffer() = delete;
    explicit IndexBuffer(const unsigned int *indices, const size_t size);
    explicit IndexBuffer(const std::vector<unsigned int> &indices);
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer &&other) noexcept;
    ~IndexBuffer();

    IndexBuffer &operator=(const IndexBuffer&) = delete;
    IndexBuffer &operator=(IndexBuffer &&other) noexcept;

    void bind() const;
    void unbind() const;

private:
    unsigned int m_id;
};


#endif //SCOP_INDEXBUFFER_HPP