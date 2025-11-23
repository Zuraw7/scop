/**
 * @file VertexArray.hpp
 * @author Patryk
 * @brief VertexArray (VAO) class declaration
 * @version 0.1
 * @date 10-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_VERTEXARRAY_HPP
#define SCOP_VERTEXARRAY_HPP
#include <GL/glew.h>

/**
 * @brief Wraps an OpenGL Vertex Array Object (VAO).
 *
 * The VertexArray class is responsible for creating, binding, and unbinding
 * a VAO in GPU memory. It stores the state needed to supply vertex data
 * to the GPU, including vertex buffers and attribute pointers.
 */
class VertexArray {
public:
    VertexArray();
    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexArray &&other) noexcept;
    ~VertexArray();

    VertexArray &operator=(const VertexArray&) = delete;
    VertexArray &operator=(VertexArray &&other) noexcept;

    void bind() const;
    void unbind() const;

private:
    unsigned int m_id;
};


#endif //SCOP_VERTEXARRAY_HPP