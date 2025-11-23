/**
 * @file Material.cpp
 * @author Patryk
 * @brief Material class implementation
 * @version 0.1
 * @date 23-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Material.hpp"

#include <fstream>
#include <memory>
#include <sstream>

/**
 * @brief Creates a material from a file.
 *
 * Loads material parameters from the given path; if loading fails,
 * initializes the material with default values.
 */
std::unique_ptr<Material> Material::create(const std::string &filePath) {
    std::unique_ptr<Material> mat = std::unique_ptr<Material>(new Material());
    if (!mat->parseFile(filePath)) {
        fprintf(stderr, "Setting default material values.\n\n");
        mat->setDefaultParams();
    }
    return mat;
}

Material::Material(Material &&other) noexcept : m_params(std::move(other.m_params)) {
}

Material &Material::operator=(Material &&other) noexcept {
    if (this == &other)
        return *this;
    m_params = std::move(other.m_params);
    return *this;
}

/**
 * @brief Sends material parameters to the shader.
 *
 * Uploads key lighting properties such as ambient, diffuse,
 * specular components and shininess to the shader uniforms.
 */
void Material::apply(Shader &shader) {
    shader.setUniformVec3("uKa", m_params.Ka);
    shader.setUniformVec3("uKd", m_params.Kd);
    shader.setUniformVec3("uKs", m_params.Ks);
    shader.setFloat("uNs", m_params.Ns);
}

/**
 * @brief Parses an MTL material file.
 *
 * Reads material parameters from the given file and fills the
 * MaterialParams structure. Returns false if the file cannot be opened.
 */
bool Material::parseFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file) {
        fprintf(stderr, "Failed to open material file %s\n", filePath.c_str());
        return false;
    }

    std::string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream ss(line);
        std::string prefix;

        ss >> prefix;
        if (prefix == "newmtl") {
            ss >> m_params.name;
        } else if (prefix == "Ns") {
            ss >> m_params.Ns;
        } else if (prefix == "Ka") {
            ss >> m_params.Ka[0]
               >> m_params.Ka[1]
               >> m_params.Ka[2];
        } else if (prefix == "Kd") {
            ss >> m_params.Kd[0]
               >> m_params.Kd[1]
               >> m_params.Kd[2];
        } else if (prefix == "Ks") {
            ss >> m_params.Ks[0]
               >> m_params.Ks[1]
               >> m_params.Ks[2];
        } else if (prefix == "Ke") {
            ss >> m_params.Ke[0]
               >> m_params.Ke[1]
               >> m_params.Ke[2];
        } else if (prefix == "Ni") {
            ss >> m_params.Ni;
        } else if (prefix == "d") {
            ss >> m_params.opacity;
        } else if (prefix == "illum") {
            ss >> m_params.illum;
        }
    }
    return true;
}

/**
 * @brief Sets default material parameters.
 *
 * Initializes all material properties with safe fallback values
 * used when loading from file fails.
 */
void Material::setDefaultParams() {
    m_params.name = "defaultParamsSettings";
    m_params.Ns = 10.0f;
    m_params.Ka = {0.0f, 0.0f, 0.0f};
    m_params.Kd = {0.8f, 0.8f, 0.8f};
    m_params.Ks = {0.0f, 0.0f, 0.0f};
    m_params.Ke = {0.0f, 0.0f, 0.0f};
    m_params.opacity = 1.0f;
    m_params.Ni = 1.0f;
    m_params.illum = 2;
}
