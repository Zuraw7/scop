/**
 * @file Renderer.cpp
 * @author Patryk
 * @brief Renderer class implementation
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Renderer.hpp"
#include "../core/Camera.hpp"
#include "../textures/TextureManager.hpp"

extern std::unique_ptr<TextureManager> gTextureManager;
extern Camera gCamera;

/**
 * @brief Creates background color from given RGBA value
 * @param red
 * @param green
 * @param blue
 * @param alpha
 */
void Renderer::setBackgroundColor(const float red, const float green, const float blue, const float alpha) {
    glClearColor(red, green, blue, alpha);
}

/**
 * @brief Clear last scene buffers
 */
void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Draws given object using given shader. Binds all necessary object's information required by shader.
 * @param object An actual object to draw
 * @param shader Program that tells how to draw given object
 * @param deltaTime Used to create smooth transition when switching mode from colored to texture
 */
void Renderer::draw(std::unique_ptr<Object> &object, Shader &shader, float deltaTime) const {
    static float rColorMix = 1.0f;

    object->bind();
    shader.bind();

    setUniforms(object, shader);

    if (m_colorMode && rColorMix < 1.0f) rColorMix += 2.0f * deltaTime;
    if (!m_colorMode && rColorMix > 0.0f) rColorMix -= 2.0f * deltaTime;

    shader.setFloat("uColorMix", rColorMix);

    glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode ? GL_LINE : GL_FILL);

    glDrawElements(GL_TRIANGLES, object->getIndices().size(), GL_UNSIGNED_INT, nullptr);

    object->unbind();
    shader.unbind();
}

void Renderer::switchPolygonMode() {
    m_polygonMode = !m_polygonMode;
}

void Renderer::toggleColorMode() {
    m_colorMode = !m_colorMode;
}

/**
 * @brief Binds objects information to shader.
 * @param object An actual object to draw
 * @param shader Program that tells how to draw given object
 */
void Renderer::setUniforms(std::unique_ptr<Object> &object, Shader &shader) const {
    shader.setUniformMatrix4fv("uModel", object->getMatrix());
    shader.setUniformMatrix4fv("uView", gCamera.getCamView());
    shader.setUniformMatrix4fv("uProjection", gCamera.getCamProjection());
    shader.setUniformVec3("uCameraPos", gCamera.getPosition());

    std::string texturePath = object->getTexture2DPath();
    gTextureManager->bindTexture(texturePath);
    shader.setInt("aTexture", gTextureManager->getSlot(texturePath));

    object->getMaterial()->apply(shader);
}
