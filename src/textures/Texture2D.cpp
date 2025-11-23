/**
 * @file Texture2D.cpp
 * @author Patryk
 * @brief Texture2D class implementation
 * @version 0.1
 * @date 11-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <GL/glew.h>
#include "Texture2D.hpp"

/**
 * @brief Loads an image from a file and creates a 2D OpenGL texture.
 *
 * This static function creates a new Texture2D object, loads the image from the specified file
 * using stb_image, uploads it to the GPU, sets wrapping and filtering options, and generates mipmaps.
 * If the image fails to load, the function returns `nullptr`.
 *
 * @param path Path to the texture image file.
 * @param wrapS Wrapping mode for the S (X) coordinate. Default: GL_MIRRORED_REPEAT.
 * @param wrapT Wrapping mode for the T (Y) coordinate. Default: GL_MIRRORED_REPEAT.
 * @param minFilter Minification filter. Default: GL_LINEAR_MIPMAP_LINEAR.
 * @param magFilter Magnification filter. Default: GL_LINEAR.
 * @return std::shared_ptr<Texture2D> Shared pointer to the loaded texture, or `nullptr` if loading failed.
 */
std::shared_ptr<Texture2D> Texture2D::create(const std::string &path, unsigned int wrapS, unsigned int wrapT, unsigned int minFilter, unsigned int magFilter) {
    std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
    texture->m_path = path;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texture->m_path.c_str(), &texture->m_width, &texture->m_height, &texture->m_nrChannels, 0);
    if (!data)
        return nullptr;

    glGenTextures(1, &texture->m_id);
    glBindTexture(GL_TEXTURE_2D, texture->m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    const unsigned int format = (texture->m_nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->m_width, texture->m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

Texture2D::Texture2D(Texture2D &&other) noexcept : m_id(other.m_id),
                                                   m_path(std::move(other.m_path)),
                                                   m_width(other.m_width),
                                                   m_height(other.m_height),
                                                   m_nrChannels(other.m_nrChannels) {
    other.m_id = 0;
    other.m_width = 0;
    other.m_height = 0;
    other.m_nrChannels = 0;
}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
    if (this != &other) {
        m_id = other.m_id;
        m_width = other.m_width;
        m_height = other.m_height;
        m_nrChannels = other.m_nrChannels;
        m_path = std::move(other.m_path);
        other.m_id = 0;
        other.m_width = 0;
        other.m_height = 0;
        other.m_nrChannels = 0;
    }
    return *this;
}

Texture2D::~Texture2D() {
    if (m_id != 0)
        glDeleteTextures(1, &m_id);
}

/**
 * @brief Binds the texture to a specific texture slot in the GPU.
 *
 * @param slot Texture slot to bind to (default is 0).
 */
void Texture2D::bind(const unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

/**
 * @brief Returns the file path of the texture.
 * @return std::string Path to the texture image file.
 */
std::string Texture2D::getPath() const {
    return m_path;
}
