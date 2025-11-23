/**
 * @file TextureManager.hpp
 * @author Patryk
 * @brief TextureManager class declaration
 * @version 0.1
 * @date 11-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_TEXTUREMANAGER_HPP
#define SCOP_TEXTUREMANAGER_HPP
#include <memory>
#include <string>
#include <unordered_map>

#include "Texture2D.hpp"

/**
 * @brief Manages loading and binding of textures.
 *
 * TextureManager handles loading textures from files, storing them,
 * and binding them to GPU texture slots. It ensures that the same texture
 * is not loaded multiple times and keeps track of assigned slots.
 */
class TextureManager {
public:
    TextureManager();
    TextureManager(const TextureManager &other);
    TextureManager(TextureManager &&other) noexcept;
    ~TextureManager() = default;

    TextureManager &operator=(const TextureManager &other);
    TextureManager &operator=(TextureManager &&other) noexcept;

    std::shared_ptr<Texture2D> loadTexture2D(const std::string &path, unsigned int wrapS = GL_MIRRORED_REPEAT,
              unsigned int wrapT = GL_MIRRORED_REPEAT,
              unsigned int minFilter = GL_LINEAR_MIPMAP_LINEAR,
              unsigned int magFilter = GL_LINEAR);

    void bindTexture(const std::string &path);
    unsigned int getSlot(const std::string &path);

private:
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures2D;
    std::unordered_map<std::string, unsigned int> m_slots;
    unsigned int m_nextSlot = 0;
    unsigned int m_maxSlots;
};


#endif //SCOP_TEXTUREMANAGER_HPP