/**
 * @file Shader.cpp
 * @author Patryk
 * @brief Shader class implementation
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <fstream>
#include <sstream>
#include "Shader.hpp"

/**
 * @brief Reads the contents of a shader file and returns it as a string.
 * @param path Path to the shader file.
 * @return std::string Contents of the shader file, or an empty string if the file could not be opened.
 */
std::string parseShaderFile(const std::string &path) {
    std::ifstream file(path.c_str());

    if (!file.is_open()) {
        fprintf(stderr, "Error opening file!\n");
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

/**
 * @brief Checks the compilation status of a shader and prints errors if any occurs.
 * @param shader OpenGL shader ID.
 * @param type Type of the shader (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).
 */
void checkShaderCompilationStatus(unsigned int shader, unsigned int type) {
    const char *shaderName;
    switch (type) {
        case GL_VERTEX_SHADER:
            shaderName = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            shaderName = "fragment";
            break;
        default:
            shaderName = "unknown";
    }

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        fprintf(stderr, "Error compiling %s shader: %s\n", shaderName, infoLog);
    }
}

/**
 * @brief Creates a shader program from vertex and fragment shader files.
 *
 * Compiles the vertex and fragment shaders, links them into an OpenGL program,
 * and sets the program as active. Shader objects are deleted after linking.
 *
 * @param vertex Path to the vertex shader source file.
 * @param fragment Path to the fragment shader source file.
 */
Shader::Shader(const std::string &vertex, const std::string &fragment) : m_id(0), m_vertexShader(0), m_fragmentShader(0) {
    m_vertexShader = compileShader(vertex, GL_VERTEX_SHADER);
    m_fragmentShader = compileShader(fragment, GL_FRAGMENT_SHADER);

    m_id = glCreateProgram();
    glAttachShader(m_id, m_vertexShader);
    glAttachShader(m_id, m_fragmentShader);
    glLinkProgram(m_id);
    glUseProgram(m_id);

    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
}

Shader::Shader(Shader &&other) noexcept : m_id(other.m_id), m_vertexShader(other.m_vertexShader), m_fragmentShader(other.m_fragmentShader) {
    other.m_id = 0;
    other.m_vertexShader = 0;
    other.m_fragmentShader = 0;
}

Shader::~Shader() {
    if (m_id)
        glDeleteProgram(m_id);
}

Shader & Shader::operator=(Shader &&other) noexcept {
    if (this == &other)
        return *this;
    m_id = other.m_id;
    m_vertexShader = other.m_vertexShader;
    m_fragmentShader = other.m_fragmentShader;
    other.m_id = 0;
    other.m_vertexShader = 0;
    other.m_fragmentShader = 0;
    return *this;
}

/**
 * @brief Binds the shader program for use in the current OpenGL context.
 */
void Shader::bind() const {
    glUseProgram(m_id);
}

/**
 * @brief Unbinds the shader program from the current OpenGL context.
 */
void Shader::unbind() const {
    glUseProgram(0);
}

/**
 * @brief Returns shader program ID.
 * @return shader program ID
 */
unsigned int Shader::getId() const {
    return m_id;
}

/**
 * @brief Sets a 4x4 float matrix uniform in the shader program.
 * @param name Name of the uniform variable.
 * @param matrix 16-element array representing the matrix.
 */
void Shader::setUniformMatrix4fv(const std::string &name, const std::array<float, 16> &matrix) {
    bind();

    const int location = findLoc(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data());
}

/**
 * @brief Sets a vec3 uniform in the shader program.
 *
 * This function binds the shader program and sets the value of a uniform
 * variable of type `vec3` using the provided 3-component float array.
 *
 * @param name Name of the uniform variable in the shader.
 * @param vec Array of three floats representing the vec3 value to set.
 */
void Shader::setUniformVec3(const std::string &name, const std::array<float, 3> &vec) {
    bind();

    const int location = findLoc(name);
    glUniform3f(location, vec[0], vec[1], vec[2]);
}

/**
 * @brief Sets an integer uniform in the shader program.
 * @param name Name of the uniform variable.
 * @param value Integer value to set.
 */
void Shader::setInt(const std::string &name, int value) {
    bind();

    const int location = findLoc(name);
    glUniform1i(location, value);
}

/**
 * @brief Sets a float uniform in the shader program.
 * @param name Name of the uniform variable.
 * @param value Float value to set.
 */
void Shader::setFloat(const std::string &name, float value) {
    bind();

    const int location = findLoc(name);
    glUniform1f(location, value);
}

/**
 * @brief Compiles a shader from a source file.
 *
 * Reads the shader source from the given file, compiles it in OpenGL,
 * and checks for compilation errors.
 *
 * @param path Path to the shader source file.
 * @param type Type of the shader (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).
 * @return unsigned int OpenGL shader ID, or 0 if compilation failed.
 */
unsigned int Shader::compileShader(const std::string &path, unsigned int type) {
    std::string source = parseShaderFile(path);
    if (source.empty()) {
        fprintf(stderr, "Shader file '%s' is empty!\n", path.c_str());
        return 0;
    }

    const char *src = source.c_str();
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    checkShaderCompilationStatus(shader, type);

    return shader;
}

/**
 * @brief Finds the location of a uniform variable in the shader program.
 *
 * Checks the cache first; if not found, queries OpenGL and caches the result.
 *
 * @param name Name of the uniform variable.
 * @return int Location of the uniform in the shader program.
 */
int Shader::findLoc(const std::string &name) {
    int location;
    const auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        location = m_uniformCache[name];
    } else {
        location = glGetUniformLocation(m_id, name.c_str());
        m_uniformCache[name] = location;
    }
    return location;
}
