#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position)
    , m_worldUp(up)
    , m_yaw(yaw)
    , m_pitch(pitch)
    , m_movementSpeed(3.0f)
    , m_mouseSensitivity(0.1f)
    , m_zoom(45.0f)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processKeyboard(char direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;

    // Create a front vector that's parallel to the ground for walking
    glm::vec3 flatFront = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
    glm::vec3 flatRight = glm::normalize(glm::vec3(m_right.x, 0.0f, m_right.z));

    switch (direction) {
        case 'W':
            m_position += flatFront * velocity;
            break;
        case 'S':
            m_position -= flatFront * velocity;
            break;
        case 'A':
            m_position -= flatRight * velocity;
            break;
        case 'D':
            m_position += flatRight * velocity;
            break;
        case ' ': // Space - jump (could be implemented)
            // For now, we'll leave this empty for future implementation
            break;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // Also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
