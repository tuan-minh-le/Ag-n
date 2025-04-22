#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 1.7f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, float pitch = 0.0f);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() const;

    // Returns the current camera position
    glm::vec3 getPosition() const { return m_position; }

    // Returns the front vector (useful for shooting direction)
    glm::vec3 getFront() const { return m_front; }

    // Processes input received from keyboard/mouse
    void processKeyboard(char direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Camera attributes
    float getZoom() const { return m_zoom; }

    // Camera configuration parameters
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

private:
    // Camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    // Euler Angles
    float m_yaw;
    float m_pitch;

    // Camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;

    // Private helper function
    void updateCameraVectors();
};
