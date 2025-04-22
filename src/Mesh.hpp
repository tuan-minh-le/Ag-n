#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "VertexBuffer.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    // Set mesh data
    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    void setTextures(const std::vector<Texture>& textures);

    // Get mesh data
    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<unsigned int>& getIndices() const { return m_indices; }
    const std::vector<Texture>& getTextures() const { return m_textures; }

    // Rendering
    void draw() const;

    // Setup the mesh for rendering
    void setupMesh() const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    // Render data (mutable to allow lazy initialization in const methods)
    mutable unsigned int m_VAO;
    mutable VertexBuffer* m_VBO;
    mutable VertexBuffer* m_EBO;

    mutable bool m_isSetup;

};
