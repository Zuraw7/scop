/**
 * @file Material.hpp
 * @author Patryk
 * @brief Material class declaration
 * @version 0.1
 * @date 17-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SCOP_MATERIAL_HPP
#define SCOP_MATERIAL_HPP
#include <array>
#include <memory>
#include "../graphics/Shader.hpp"

/**
 * @brief Stores basic material properties.
 *
 * Contains lighting parameters such as ambient, diffuse, specular,
 * emissive colors, shininess, opacity and illumination model.
 */
struct MaterialParams {
    std::string name;
    float Ns;
    std::array<float, 3> Ka;
    std::array<float, 3> Kd;
    std::array<float, 3> Ks;
    std::array<float, 3> Ke;
    float Ni;
    float opacity;
    float illum;
};

/**
 * @brief Loads and applies material parameters.
 *
 * Parses a material file, stores its parameters and uploads them
 * to a shader for rendering.
 */
class Material {
public:
    Material() = default;
    ~Material() = default;
    Material(const Material &other) = delete;
    Material(Material &&other) noexcept;
    Material &operator=(const Material &other) = delete;
    Material &operator=(Material &&other) noexcept;

    static std::unique_ptr<Material> create(const std::string &filePath);
    void apply(Shader &shader);

private:
    MaterialParams m_params;

    bool parseFile(const std::string &filePath);
    void setDefaultParams();
};


#endif //SCOP_MATERIAL_HPP