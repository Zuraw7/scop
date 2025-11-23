/**
 * @file Object.cpp
 * @author Patryk
 * @brief Object class implementation
 * @version 0.1
 * @date 13-11-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "./Object.hpp"
#include "../textures/Material.hpp"

/**
 * @brief Creates and initializes an Object from a .obj file.
 *
 * This static function performs the following steps:
 * 1. Allocates a new Object instance on the heap.
 * 2. Parses the specified .obj file using `parseFile()`. Returns `nullptr` if parsing fails.
 * 3. Initializes OpenGL buffers (VAO/VBO/IBO) via `initBuffers()`.
 * 4. Calculates the geometric center of the object and its scale factor.
 * 5. Sets the object's transformation matrices (`m_matrix`, `m_translationMatrix`, `m_rotationMatrix`) to identity.
 *
 * @param objFilePath Path to the .obj file to load.
 * @return std::unique_ptr<Object> Returns a unique pointer to the fully initialized Object on success, or `nullptr` if the file could not be parsed.
 */
std::unique_ptr<Object> Object::create(const std::string &objFilePath) {
    std::unique_ptr<Object> obj(new Object());

    if (obj->parseFile(objFilePath)) {
        fprintf(stderr, "Failed to load object: %s\n", objFilePath.c_str());
        return nullptr;
    }

    obj->initBuffers();

    obj->m_center = obj->calculateCenter();
    obj->m_scaleFactor = 1.0f / obj->calculateScale();

    obj->m_matrix = getIdentityMat4();
    obj->m_translationMatrix = getIdentityMat4();
    obj->m_rotationMatrix = getIdentityMat4();

    obj->m_material = Material::create(objFilePath.substr(0, objFilePath.rfind('.')) + ".mtl");
    return obj;
}

Object::Object(Object &&other) noexcept : m_vertices(std::move(other.m_vertices)),
                                          m_indices(std::move(other.m_indices)),
                                          m_center(other.m_center),
                                          m_translationMatrix(other.m_translationMatrix),
                                          m_rotationMatrix(other.m_rotationMatrix),
                                          m_matrix(other.m_matrix),
                                          m_scaleFactor(other.m_scaleFactor),
                                          m_VAO(std::move(other.m_VAO)),
                                          m_VBO(std::move(other.m_VBO)),
                                          m_IBO(std::move(other.m_IBO)) {
}

Object &Object::operator=(Object &&other) noexcept {
    if (this == &other)
        return *this;
    m_vertices = std::move(other.m_vertices);
    m_indices = std::move(other.m_indices);
    m_center = other.m_center;
    m_matrix = other.m_matrix;
    m_rotationMatrix = other.m_rotationMatrix;
    m_translationMatrix = other.m_translationMatrix;
    m_scaleFactor = other.m_scaleFactor;
    m_VAO = std::move(other.m_VAO);
    m_VBO = std::move(other.m_VBO);
    m_IBO = std::move(other.m_IBO);
    return *this;
}

/**
 * @brief Binds the object's Vertex Array Object (VAO) for rendering.
 */
void Object::bind() const {
    m_VAO.bind();
}

/**
 * @brief Unbinds the object's Vertex Array Object (VAO).
 */
void Object::unbind() const {
    m_VAO.unbind();
}

/**
 * @brief Updates the object's rotation around its local Y axis.
 *
 * Rotates the object around its center by the given angle. The rotation
 * is accumulated with the existing rotation matrix, allowing multiple
 * rotations to be applied sequentially around the object's own center.
 *
 * @param angle Rotation angle in degrees.
 */
void Object::updateRotationMatrixY(const float angle) {
    // Move the object so that its center (m_center) is at (0,0,0) – rotation will now be around the object's center
    auto centeredMatrix = translateMatrix(getIdentityMat4(), -m_center[0], -m_center[1], -m_center[2]);

    // Create a rotation matrix around the Y axis
    auto rotationMatrix = getRotationMatrixY(angle);

    // Move the object back to its original position in local coordinates
    auto originMatrix = translateMatrix(getIdentityMat4(), m_center[0], m_center[1], m_center[2]);

    // Combine the translation to center, rotation, and translation back into a single matrix
    // The resulting matrix rotates the object around its own center
    auto rotAroundCenter = multiplyMatrix(centeredMatrix, multiplyMatrix(rotationMatrix, originMatrix));

    // Accumulate the new rotation with the previous rotation – each subsequent rotation occurs around the object's center
    m_rotationMatrix = multiplyMatrix(rotAroundCenter, m_rotationMatrix);
}

/**
 * @brief Moves the object along the X axis.
 * @param direction Movement direction.
 * @param deltaTime Time elapsed since the last frame.
 */
void Object::moveXaxis(const float direction, const double deltaTime) {
    m_translationMatrix[12] += MOVE_SPEED * direction * deltaTime;
}

/**
 * @brief Moves the object along the Y axis.
 * @param direction Movement direction.
 * @param deltaTime Time elapsed since the last frame.
 */
void Object::moveYaxis(const float direction, const double deltaTime) {
    m_translationMatrix[13] += MOVE_SPEED * direction * deltaTime;
}

/**
 * @brief Moves the object along the Z axis.
 * @param direction Movement direction.
 * @param deltaTime Time elapsed since the last frame.
 */
void Object::moveZaxis(const float direction, const double deltaTime) {
    m_translationMatrix[14] += MOVE_SPEED * direction * deltaTime;
}

const VertexArray &Object::getVAO() const {
    return m_VAO;
}

std::array<float, 3> Object::getCenter() const {
    return m_center;
}

std::vector<unsigned int> Object::getIndices() const {
    return m_indices;
}

/**
 * @brief Computes and returns the object's model transformation matrix.
 *
 * Combines translation, rotation, scaling, and center offset to produce
 * the final 4x4 model matrix used for rendering the object in world space.
 *
 * @return std::array<float, 16> The object's model matrix.
 */
std::array<float, 16> Object::getMatrix() {
    m_matrix = translateMatrix(getIdentityMat4(), -m_center[0], -m_center[1], -m_center[2]);
    m_matrix = scaleMatrix(m_matrix, m_scaleFactor);
    m_matrix = multiplyMatrix(m_matrix, m_translationMatrix);
    m_matrix = multiplyMatrix(m_rotationMatrix, m_matrix);

    return m_matrix;
}

std::string Object::getTexture2DPath() const {
    return m_texture2D.get()->getPath();
}

const std::array<float, 3> Object::getPosition() const {
    return { m_translationMatrix[12], m_translationMatrix[13], m_translationMatrix[14] };
}

const std::unique_ptr<Material> &Object::getMaterial() {
    return m_material;
}

void Object::setTexture2D(const std::shared_ptr<Texture2D> &texture) {
    m_texture2D = texture;
}

/**
 * @brief Loads vertex and face data from an .obj file.
 *
 * Parses the given .obj file line by line. Vertex positions (`v`) are stored
 * in m_vertices, and faces (`f`) are converted into triangle indices stored
 * in m_indices. Quad faces are automatically split into two triangles.
 * After parsing, texture UV coordinates are calculated using the ZY projection.
 *
 * @param filePath The path to the .obj file to parse.
 * @return int Returns 0 if the file was successfully parsed and contains valid vertices and indices. Returns 1 if the file could not be opened or contains no valid geometry.
 */
int Object::parseFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file) {
        fprintf(stderr, "Failed to open file %s\n", filePath.c_str());
        return 1;
    }

    std::string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            ss >> x >> y >> z;

            Vertex v{};
            v.position = {x, y, z};
            v.uv = {0.0f, 0.0f};
            m_vertices.push_back(v);
        } else if (prefix == "f") {
            std::vector<unsigned int> indices;
            unsigned int index;
            while (ss >> index)
                indices.push_back(index - 1);

            if (indices.size() == 3) {
                m_indices.push_back(indices[0]);
                m_indices.push_back(indices[1]);
                m_indices.push_back(indices[2]);
            } else if (indices.size() == 4) {
                m_indices.push_back(indices[0]);
                m_indices.push_back(indices[1]);
                m_indices.push_back(indices[2]);

                m_indices.push_back(indices[0]);
                m_indices.push_back(indices[2]);
                m_indices.push_back(indices[3]);
            }
        }
    }
    if (m_vertices.empty() || m_indices.empty())
        return 1;
    calculateUV_XY();
    computeNormals();
    return 0;
}

/**
 * @brief Initializes OpenGL buffers for the object.
 *
 * Creates and binds the Vertex Array Object (VAO), Vertex Buffer Object (VBO),
 * and Index Buffer Object (IBO). Sets up vertex attribute pointers for position
 * and texture coordinates, which are used by shaders during rendering.
 * After initialization, all buffers are unbound.
 */
void Object::initBuffers() {
    m_VAO.bind();

    m_VBO = std::unique_ptr<VertexBuffer>(new VertexBuffer(m_vertices));
    m_IBO = std::unique_ptr<IndexBuffer>(new IndexBuffer(m_indices));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    m_VAO.unbind();
    m_VBO->unbind();
    m_IBO->unbind();
}

/**
 * @brief Calculates the geometric center of the object.
 *
 * Computes the average position of all vertices to determine the object's
 * center in local coordinates.
 *
 * @return std::array<float, 3> The computed center {x, y, z}.
 */
std::array<float, 3> Object::calculateCenter() const {
    float sumX = 0, sumY = 0, sumZ = 0;
    const size_t count = m_vertices.size();

    for (const auto &v: m_vertices) {
        sumX += v.position[0];
        sumY += v.position[1];
        sumZ += v.position[2];
    }

    return { (sumX / count), (sumY / count), (sumZ / count) };
}

/**
 * @brief Calculates the object's bounding scale.
 *
 * Determines the size of the object's bounding box along each axis and
 * returns the largest dimension. This scale factor can be used to
 * normalize the object size for consistent rendering.
 *
 * @return float The maximum dimension of the object (scale factor).
 */
float Object::calculateScale() const {
    if (m_vertices.empty()) return 0.0f;

    const auto v = m_vertices.data();
    std::array<float, 3> min = {v->position[0], v->position[1], v->position[2]};
    std::array<float, 3> max = {v->position[0], v->position[1], v->position[2]};

    for (const auto &vp: m_vertices) {
        const float x = vp.position[0];
        const float y = vp.position[1];
        const float z = vp.position[2];

        if (x > max[0]) max[0] = x;
        if (y > max[1]) max[1] = y;
        if (z > max[2]) max[2] = z;

        if (x < min[0]) min[0] = x;
        if (y < min[1]) min[1] = y;
        if (z < min[2]) min[2] = z;
    }
    const float sizeX = max[0] - min[0];
    const float sizeY = max[1] - min[1];
    const float sizeZ = max[2] - min[2];

    return std::max( {sizeX, sizeY, sizeZ} );
}

/**
 * @brief Calculates UV coordinates using XY plane projection.
 *
 * Computes normalized texture coordinates for each vertex based on their
 * X and Y positions. The minimum and maximum X/Y values are used to map
 * positions into the [0, 1] UV range.
 */
void Object::calculateUV_XY() {
    if (m_vertices.empty()) return;

    float minX = m_vertices[0].position[0], maxX = m_vertices[0].position[0];
    float minY = m_vertices[0].position[1], maxY = m_vertices[0].position[1];

    for (const auto &v: m_vertices) {
        if (v.position[0] < minX) minX = v.position[0];
        if (v.position[0] > maxX) maxX = v.position[0];
        if (v.position[1] < minY) minY = v.position[1];
        if (v.position[1] > maxY) maxY = v.position[1];

    }
    for (auto &v: m_vertices) {
        v.uv[0] = (v.position[0] - minX) / (maxX - minX);
        v.uv[1] = (v.position[1] - minY) / (maxY - minY);
    }
}

/**
 * @brief Calculates UV coordinates using ZY plane projection.
 *
 * Computes normalized texture coordinates for each vertex based on their
 * Z and Y positions. The minimum and maximum Z/Y values are used to map
 * positions into the [0, 1] UV range.
 */
void Object::calculateUV_ZY() {
    if (m_vertices.empty()) return;

    float minZ = m_vertices[0].position[2], maxZ = m_vertices[0].position[2];
    float minY = m_vertices[0].position[1], maxY = m_vertices[0].position[1];

    for (const auto &v : m_vertices) {
        if (v.position[2] < minZ) minZ = v.position[2];
        if (v.position[2] > maxZ) maxZ = v.position[2];
        if (v.position[1] < minY) minY = v.position[1];
        if (v.position[1] > maxY) maxY = v.position[1];
    }

    for (auto &v : m_vertices) {
        v.uv[0] = (v.position[2] - minZ) / (maxZ - minZ);
        v.uv[1] = (v.position[1] - minY) / (maxY - minY);
    }
}

/**
 * @brief Computes smooth normals for all vertices of the object.
 *
 * Calculates each vertex normal by averaging the normals of all
 * triangles that share the vertex. The result is used for lighting
 * and shading in 3D rendering.
 */
void Object::computeNormals() {
    for (auto &v: m_vertices) {
        v.normal = {0.0f, 0.0f, 0.0f};
    }

    for (size_t i = 0; i < m_indices.size(); i += 3) {
        const unsigned int i0 = m_indices[i];
        const unsigned int i1 = m_indices[i + 1];
        const unsigned int i2 = m_indices[i + 2];

        std::array<float, 3> &v0 = m_vertices[i0].position;
        std::array<float, 3> &v1 = m_vertices[i1].position;
        std::array<float, 3> &v2 = m_vertices[i2].position;

        std::array<float, 3> edge1 = { v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2] };
        std::array<float, 3> edge2 = { v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2] };

        std::array<float, 3> faceNormal = crossProdVec(edge1, edge2);
        faceNormal = normalizeVec(faceNormal);

        unsigned int indices[3] = { i0, i1, i2 };
        for (const unsigned int idx: indices) {
            m_vertices[idx].normal[0] += faceNormal[0];
            m_vertices[idx].normal[1] += faceNormal[1];
            m_vertices[idx].normal[2] += faceNormal[2];
        }
    }
    for (auto &v: m_vertices) {
        v.normal = normalizeVec(v.normal);
    }
}
