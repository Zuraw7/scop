/**
 * @file Texture2D.hpp
 * @author Patryk
 * @brief Texture2D class declaration
 * @version 0.1
 * @date 11-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_TEXTURE_HPP
#define SCOP_TEXTURE_HPP

#include "../3rd/stb_image.h"
#include <iostream>
#include <memory>
#include <GL/glew.h>

/**
 * @brief Wraps a 2D texture in OpenGL.
 *
 * The Texture2D class loads an image from a file and creates
 * an OpenGL texture. It supports setting wrapping and filtering options.
 */
class Texture2D {
public:
    Texture2D() = default;
    static std::shared_ptr<Texture2D> create(const std::string &path,
              unsigned int wrapS = GL_MIRRORED_REPEAT,
              unsigned int wrapT = GL_MIRRORED_REPEAT,
              unsigned int minFilter = GL_LINEAR_MIPMAP_LINEAR,
              unsigned int magFilter = GL_LINEAR);
    Texture2D(const Texture2D &other) = delete;
    Texture2D(Texture2D &&other) noexcept;

    Texture2D &operator=(const Texture2D &other) = delete;
    Texture2D &operator=(Texture2D &&other) noexcept;

    ~Texture2D();

    void bind(const unsigned int slot = 0) const;

    std::string getPath() const;

private:
    unsigned int m_id;
    std::string m_path;
    int m_width, m_height, m_nrChannels;
};


#endif //SCOP_TEXTURE_HPP