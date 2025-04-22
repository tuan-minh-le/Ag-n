#include "Mesh.hpp"
#include <GL/glew.h>

Mesh::Mesh()
    : m_VAO(0)
    , m_VBO(nullptr)
    , m_EBO(nullptr)
    , m_isSetup(false)
{
}

Mesh::~Mesh() {
    if (m_VBO) delete m_VBO;
    if (m_EBO) delete m_EBO;
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::setVertices(const std::vector<Vertex>& vertices) {
    m_vertices = vertices;
    m_isSetup = false;
}

void Mesh::setIndices(const std::vector<unsigned int>& indices) {
    m_indices = indices;
    m_isSetup = false;
}

void Mesh::setTextures(const std::vector<Texture>& textures) {
    m_textures = textures;
}

void Mesh::setupMesh() const {
    if (m_isSetup) return;

    // Create VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create VBO
    m_VBO = new VertexBuffer();
    m_VBO->bind();
    m_VBO->setData(m_vertices.data(), m_vertices.size() * sizeof(Vertex));

    // Create EBO
    m_EBO = new VertexBuffer(GL_ELEMENT_ARRAY_BUFFER);
    m_EBO->bind();
    m_EBO->setData(m_indices.data(), m_indices.size() * sizeof(unsigned int));

    // Set vertex attribute pointers
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Texture coords attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);

    m_isSetup = true;
}

void Mesh::draw() const {
    if (!m_isSetup) {
        setupMesh();
    }

    // Bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = m_textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }

    // Draw mesh
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Reset to default texture
    glActiveTexture(GL_TEXTURE0);
}
