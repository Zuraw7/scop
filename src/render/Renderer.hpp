/**
 * @file Renderer.hpp
 * @author Patryk
 * @brief Renderer class declaration
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_RENDERER_HPP
#define SCOP_RENDERER_HPP

#include "../core/Object.hpp"
#include "../graphics/Shader.hpp"

class Object;

/**
 * @brief Renderer wraps all rendering calls into dedicated functions. It also controlls how objects are being rendered.
 */
class Renderer {
public:
    void setBackgroundColor(const float red, const float green, const float blue, const float alpha);
    void clear() const;
    void draw(std::unique_ptr<Object> &object, Shader& shader, float deltaTime) const;
    void switchPolygonMode();
    void toggleColorMode();

private:
    void setUniforms(std::unique_ptr<Object> &object, Shader& shader) const;
    bool m_polygonMode = false;
    bool m_colorMode = true;
};

#endif //SCOP_RENDERER_HPP
