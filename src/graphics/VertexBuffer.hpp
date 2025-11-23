/**
 * @file VertexBuffer.hpp
 * @author Patryk
 * @brief VertexBuffer (VBO) class declaration
 * @version 0.1
 * @date 11-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_VERTEXBUFFER_HPP
#define SCOP_VERTEXBUFFER_HPP
#include <vector>
#include <GL/glew.h>

struct Vertex;

/**
 * @brief Wraps an OpenGL Vertex Buffer Object (VBO).
 *
 * The VertexBuffer class is responsible for creating, binding, and unbinding
 * a VBO in GPU memory. It stores vertex data for rendering and supports
 * construction from raw arrays, std::vector of floats, or std::vector of Vertex structs.
 */
class VertexBuffer {
public:
    VertexBuffer() = delete;
    explicit VertexBuffer(const float *vertices, const size_t size);
    explicit VertexBuffer(const std::vector<float> &vertices);
    explicit VertexBuffer(const std::vector<Vertex> &vertices);
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer &&other) noexcept;
    ~VertexBuffer();

    VertexBuffer &operator=(const VertexBuffer&) = delete;
    VertexBuffer &operator=(VertexBuffer &&other) noexcept;

    void bind() const;
    void unbind() const;

private:
    unsigned int m_id;
};


#endif //SCOP_VERTEXBUFFER_HPP