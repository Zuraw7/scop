/**
 * @file Shader.hpp
 * @author Patryk
 * @brief Shader class declaration
 * @version 0.1
 * @date 11-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_SHADER_HPP
#define SCOP_SHADER_HPP
#include <iostream>
#include <array>
#include <unordered_map>
#include <GL/glew.h>

/**
 * @brief Wraps an OpenGL Shader Program.
 *
 * The Shader class is responsible for loading, compiling, and linking
 * vertex and fragment shaders into an OpenGL shader program.
 * It also provides methods to bind/unbind the program and set uniform variables.
 */
class Shader {
public:
    Shader(const std::string &vertex, const std::string &fragment);
    Shader(const Shader& other) = delete;
    Shader(Shader &&other) noexcept;
    ~Shader();

    Shader &operator=(const Shader &other) = delete;
    Shader &operator=(Shader &&other) noexcept;

    void bind() const;
    void unbind() const;
    unsigned int getId() const;

    void setUniformMatrix4fv(const std::string &name, const std::array<float, 16> &matrix);
    void setUniformVec3(const std::string &name, const std::array<float, 3> &vec);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);

private:
    unsigned int m_id;
    std::unordered_map<std::string, int> m_uniformCache;
    unsigned int m_vertexShader;
    unsigned int m_fragmentShader;

    unsigned int compileShader(const std::string &path, unsigned int type);
    int findLoc(const std::string &name);
};


#endif //SCOP_SHADER_HPP