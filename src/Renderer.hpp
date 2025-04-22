#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"
#include "Mesh.hpp"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    // Initialize OpenGL context
    bool initialize();

    // Clear the screen
    void clear();

    // Draw a mesh
    void drawMesh(const Mesh* mesh, const glm::mat4& modelMatrix);

    // Set camera for rendering
    void setCamera(const Camera* camera);

    // Projection control
    void setProjection(float fov, float aspect, float near, float far);

    // Shader management
    unsigned int loadShader(const char* vertexPath, const char* fragmentPath);
    void useShader(unsigned int shaderProgram);
    void setShaderMat4(unsigned int shaderProgram, const char* name, const glm::mat4& mat);
    void setShaderVec3(unsigned int shaderProgram, const char* name, const glm::vec3& vec);
    void setShaderFloat(unsigned int shaderProgram, const char* name, float value);

    // Resize viewport
    void resize(int width, int height);

private:
    int m_width;
    int m_height;

    const Camera* m_camera;
    glm::mat4 m_projection;

    unsigned int m_defaultShader;

    // Helper functions
    unsigned int compileShader(const char* source, GLenum type);
    bool checkShaderCompileErrors(unsigned int shader);
    bool checkProgramLinkErrors(unsigned int program);
};
