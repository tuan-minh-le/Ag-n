#include "Renderer.hpp"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

Renderer::Renderer(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_camera(nullptr)
    , m_defaultShader(0)
{
    setProjection(45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

Renderer::~Renderer() {
    if (m_defaultShader) {
        glDeleteProgram(m_defaultShader);
    }
}

bool Renderer::initialize() {
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Set OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Load default shader
    m_defaultShader = loadShader("res/shaders/basic.vert", "res/shaders/basic.frag");
    if (m_defaultShader == 0) {
        std::cerr << "Failed to load default shader" << std::endl;
        return false;
    }

    return true;
}

void Renderer::clear() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawMesh(const Mesh* mesh, const glm::mat4& modelMatrix) {
    if (!m_camera) return;

    // Use default shader if no shader is explicitly set
    useShader(m_defaultShader);

    // Set matrices
    setShaderMat4(m_defaultShader, "model", modelMatrix);
    setShaderMat4(m_defaultShader, "view", m_camera->getViewMatrix());
    setShaderMat4(m_defaultShader, "projection", m_projection);

    // Draw the mesh
    mesh->draw();
}

void Renderer::setCamera(const Camera* camera) {
    m_camera = camera;
}

void Renderer::setProjection(float fov, float aspect, float near, float far) {
    m_projection = glm::perspective(glm::radians(fov), aspect, near, far);
}

unsigned int Renderer::loadShader(const char* vertexPath, const char* fragmentPath) {
    // Read vertex shader
    std::string vertexCode;
    std::ifstream vShaderFile(vertexPath);
    if (vShaderFile) {
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vertexCode = vShaderStream.str();
    } else {
        std::cerr << "Failed to read vertex shader file: " << vertexPath << std::endl;
        return 0;
    }

    // Read fragment shader
    std::string fragmentCode;
    std::ifstream fShaderFile(fragmentPath);
    if (fShaderFile) {
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fragmentCode = fShaderStream.str();
    } else {
        std::cerr << "Failed to read fragment shader file: " << fragmentPath << std::endl;
        return 0;
    }

    // Compile shaders
    unsigned int vertex = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    unsigned int fragment = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    if (vertex == 0 || fragment == 0) {
        return 0;
    }

    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    if (!checkProgramLinkErrors(shaderProgram)) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }

    // Delete shaders as they're linked into our program and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shaderProgram;
}

void Renderer::useShader(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);
}

void Renderer::setShaderMat4(unsigned int shaderProgram, const char* name, const glm::mat4& mat) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, &mat[0][0]);
}

void Renderer::setShaderVec3(unsigned int shaderProgram, const char* name, const glm::vec3& vec) {
    glUseProgram(shaderProgram);
    glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, &vec[0]);
}

void Renderer::setShaderFloat(unsigned int shaderProgram, const char* name, float value) {
    glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void Renderer::resize(int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
    setProjection(45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

unsigned int Renderer::compileShader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    if (!checkShaderCompileErrors(shader)) {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Renderer::checkShaderCompileErrors(unsigned int shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Renderer::checkProgramLinkErrors(unsigned int program) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking error: " << infoLog << std::endl;
        return false;
    }
    return true;
}
