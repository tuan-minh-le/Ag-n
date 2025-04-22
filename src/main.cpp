#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "Level.hpp"

// Window dimensions
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Global variables for callback functions
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Player pointer for callback access
Player* g_player = nullptr;

// Callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window))) {
        renderer->resize(width, height);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    if (g_player) {
        g_player->look(xoffset, yoffset);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (g_player && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        g_player->shoot();
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (g_player && action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            g_player->reload();
        } else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_3) {
            g_player->changeWeapon(key - GLFW_KEY_1);
        }
    }
}

void processInput(GLFWwindow* window) {
    if (g_player) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            g_player->move('W', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            g_player->move('S', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            g_player->move('A', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            g_player->move('D', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            g_player->move(' ', deltaTime);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ag-n - Indoor Battle Royale", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Create renderer
    Renderer renderer(SCR_WIDTH, SCR_HEIGHT);
    if (!renderer.initialize()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    // Store renderer in window user pointer for callback access
    glfwSetWindowUserPointer(window, &renderer);

    // Create level
    Level level;

    // Create player
    Player player(glm::vec3(0.0f, 0.0f, 0.0f));
    g_player = &player;

    // Set camera for renderer
    renderer.setCamera(&player.getCamera());

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Update
        player.update(deltaTime);

        // Render
        renderer.clear();

        // Draw level
        for (const auto& mesh : level.getMeshes()) {
            renderer.drawMesh(mesh, glm::mat4(1.0f));
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
