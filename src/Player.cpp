#include "Player.hpp"
#include <iostream>

Player::Player(const glm::vec3& spawnPosition)
    : m_position(spawnPosition)
    , m_camera(spawnPosition + glm::vec3(0.0f, 1.7f, 0.0f))
    , m_health(100.0f)
    , m_armor(0.0f)
    , m_speed(3.0f)
    , m_jumpForce(5.0f)
    , m_isJumping(false)
    , m_velocity(0.0f)
    , m_currentWeaponIndex(0)
    , m_height(1.8f)
    , m_radius(0.3f)
{
    // Initialize weapons (we'll create specific weapon types later)
    m_weapons[0] = new Weapon("Pistol", 30.0f, 15, 7);     // Basic pistol
    m_weapons[1] = new Weapon("Shotgun", 80.0f, 30, 2);    // Powerful but limited
    m_weapons[2] = new Weapon("SMG", 15.0f, 10, 30);       // Fast firing

    m_currentWeapon = m_weapons[0];
}

Player::~Player() {
    for (int i = 0; i < 3; i++) {
        delete m_weapons[i];
    }
}

void Player::update(float deltaTime) {
    // Apply gravity if jumping/falling
    if (m_isJumping || m_position.y > 0.0f) {
        applyGravity(deltaTime);
    }

    // Update position based on velocity
    if (m_isJumping) {
        m_position += m_velocity * deltaTime;

        // Check if landed
        if (m_position.y <= 0.0f) {
            m_position.y = 0.0f;
            m_isJumping = false;
            m_velocity.y = 0.0f;
        }
    }

    // Update camera position to follow player
    m_camera.processKeyboard(0, 0); // Update camera vectors
    m_camera = Camera(m_position + glm::vec3(0.0f, 1.7f, 0.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f),
                      m_camera.getFront().x,
                      m_camera.getFront().y);
}

void Player::move(char direction, float deltaTime) {
    float velocity = m_speed * deltaTime;
    glm::vec3 newPosition = m_position;

    // Calculate movement based on camera direction
    glm::vec3 front = m_camera.getFront();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    // Keep movement horizontal (no vertical component)
    front.y = 0.0f;
    front = glm::normalize(front);

    switch (direction) {
        case 'W':
            newPosition += front * velocity;
            break;
        case 'S':
            newPosition -= front * velocity;
            break;
        case 'A':
            newPosition -= right * velocity;
            break;
        case 'D':
            newPosition += right * velocity;
            break;
        case ' ': // Space - jump
            if (!m_isJumping && m_position.y <= 0.0f) {
                m_isJumping = true;
                m_velocity.y = m_jumpForce;
            }
            break;
    }

    // Check collision before updating position
    if (!checkCollision(newPosition)) {
        m_position = newPosition;
    }
}

void Player::look(float xoffset, float yoffset) {
    m_camera.processMouseMovement(xoffset, yoffset);
}

void Player::shoot() {
    if (m_currentWeapon && m_currentWeapon->canShoot()) {
        m_currentWeapon->shoot();

        // Ray casting for bullet hit detection would go here
        // For now, we'll just log the action
        std::cout << "Shot fired with " << m_currentWeapon->getName() << std::endl;
    }
}

void Player::reload() {
    if (m_currentWeapon) {
        m_currentWeapon->reload();
        std::cout << "Reloading " << m_currentWeapon->getName() << std::endl;
    }
}

void Player::changeWeapon(int slot) {
    if (slot >= 0 && slot < 3) {
        m_currentWeaponIndex = slot;
        m_currentWeapon = m_weapons[slot];
        std::cout << "Switched to " << m_currentWeapon->getName() << std::endl;
    }
}

void Player::takeDamage(float damage) {
    // Damage reduction based on armor
    float actualDamage = damage;
    if (m_armor > 0.0f) {
        float reduction = damage * 0.5f; // 50% damage reduction
        float armorDamage = std::min(m_armor, reduction);
        m_armor -= armorDamage;
        actualDamage = damage - armorDamage;
    }

    m_health -= actualDamage;
    if (m_health < 0.0f) {
        m_health = 0.0f;
    }
}

void Player::heal(float amount) {
    m_health = std::min(100.0f, m_health + amount);
}

void Player::addArmor(float amount) {
    m_armor = std::min(100.0f, m_armor + amount);
}

bool Player::checkCollision(const glm::vec3& position) {
    // TODO: Implement collision detection with level geometry
    // For now, just check if within bounds of the apartment
    if (position.x < -10.0f || position.x > 10.0f ||
        position.z < -10.0f || position.z > 10.0f) {
        return true; // Collision detected (hit wall)
    }
    return false;
}

void Player::applyGravity(float deltaTime) {
    const float gravity = -9.81f;
    m_velocity.y += gravity * deltaTime;
}

void Player::updateCollisionBox() {
    // Update the collision box based on player position
    // This would be used for more sophisticated collision detection
}
