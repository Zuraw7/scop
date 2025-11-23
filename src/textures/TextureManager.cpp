/**
 * @file TextureManager.cpp
 * @author Patryk
 * @brief TextureManager class implementation
 * @version 0.1
 * @date 11-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "TextureManager.hpp"

/**
 * @brief Initializes the texture manager and queries the GPU for maximum texture slots.
 *
 * Sets the next available slot to 0 and retrieves the maximum number of
 * combined texture image units supported by the GPU.
 */
TextureManager::TextureManager() : m_nextSlot(0), m_maxSlots(0) {
    int maxUnits = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);
    m_maxSlots = static_cast<unsigned int>(maxUnits);
}

TextureManager::TextureManager(const TextureManager &other) : m_textures2D(other.m_textures2D), m_slots(other.m_slots), m_nextSlot(other.m_nextSlot) {
}

TextureManager::TextureManager(TextureManager &&other) noexcept : m_textures2D(std::move(other.m_textures2D)),
      m_slots(std::move(other.m_slots)),
      m_nextSlot(other.m_nextSlot) {
}

TextureManager &TextureManager::operator=(const TextureManager &other) {
    if (this == &other)
        return *this;
    m_textures2D = other.m_textures2D;
    m_slots = other.m_slots;
    m_nextSlot = other.m_nextSlot;
    return *this;
}

TextureManager & TextureManager::operator=(TextureManager &&other) noexcept {
    if (this == &other)
        return *this;
    m_textures2D = std::move(other.m_textures2D);
    m_slots = std::move(other.m_slots);
    m_nextSlot = other.m_nextSlot;
    return *this;
}

/**
 * @brief Loads a 2D texture or returns an existing one if already loaded.
 *
 * This function checks if a texture with the given path is already loaded in the manager.
 * If it exists, the existing texture is returned. Otherwise, it attempts to create a new
 * Texture2D using `Texture2D::create()`. If creation succeeds, the texture is stored in
 * the manager, assigned a slot, and returned. If creation fails (e.g., file not found or
 * invalid format), `nullptr` is returned and an error message is printed.
 *
 * @param path Path to the texture file.
 * @param wrapS Wrapping mode for the S (X) coordinate. Default: GL_MIRRORED_REPEAT.
 * @param wrapT Wrapping mode for the T (Y) coordinate. Default: GL_MIRRORED_REPEAT.
 * @param minFilter Minification filter. Default: GL_LINEAR_MIPMAP_LINEAR.
 * @param magFilter Magnification filter. Default: GL_LINEAR.
 * @return std::shared_ptr<Texture2D> Shared pointer to the loaded texture, or `nullptr` if loading failed.
 */
std::shared_ptr<Texture2D> TextureManager::loadTexture2D(const std::string &path, unsigned int wrapS, unsigned int wrapT, unsigned int minFilter, unsigned int magFilter) {
    auto it = m_textures2D.find(path);
    if (it != m_textures2D.end()) return it->second;

    auto tex = Texture2D::create(path, wrapS, wrapT, minFilter, magFilter);
    if (!tex) {
        fprintf(stderr, "Failed to load texture: %s\n", path.c_str());
        return nullptr;
    }
    m_textures2D[path] = tex;
    m_slots[path] = m_nextSlot;
    m_nextSlot++;
    return tex;
}

/**
 * @brief Returns the GPU slot assigned to a texture.
 *
 * Looks up the slot assigned to the texture with the given path.
 * If the texture is not found, prints a warning and returns slot 0.
 *
 * @param path Path to the texture file.
 * @return unsigned int Assigned texture slot, or 0 if not found.
 */
unsigned int TextureManager::getSlot(const std::string &path) {
    auto it = m_slots.find(path);
    if (it != m_slots.end()) {
        return it->second;
    }
    fprintf(stderr, "Warning: texture %s not found, returning slot 0!\n", path.c_str());
    return 0;
}

/**
 * @brief Binds the texture with the given path to its assigned GPU slot.
 *
 * Finds the texture in the manager and binds it to the correct slot.
 * Does nothing if the texture is not found.
 *
 * @param path Path to the texture file to bind.
 */
void TextureManager::bindTexture(const std::string &path) {
    auto it = m_textures2D.find(path);
    if (it != m_textures2D.end()) {
        unsigned int slot = m_slots[path];
        it->second->bind(slot % m_maxSlots);
    }
}
