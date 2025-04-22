#pragma once

#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Weapon.hpp"

class Player {
public:
    Player(const glm::vec3& spawnPosition);
    ~Player();

    // Update player state
    void update(float deltaTime);

    // Movement control
    void move(char direction, float deltaTime);
    void look(float xoffset, float yoffset);

    // Actions
    void shoot();
    void reload();
    void changeWeapon(int slot);

    // Health and armor management
    void takeDamage(float damage);
    void heal(float amount);
    void addArmor(float amount);

    // Collision detection
    bool checkCollision(const glm::vec3& position);

    // Getters
    Camera& getCamera() { return m_camera; }
    const Camera& getCamera() const { return m_camera; }
    glm::vec3 getPosition() const { return m_position; }
    float getHealth() const { return m_health; }
    float getArmor() const { return m_armor; }
    const Weapon* getCurrentWeapon() const { return m_currentWeapon; }
    bool isAlive() const { return m_health > 0.0f; }

private:
    // Player attributes
    glm::vec3 m_position;
    Camera m_camera;
    float m_health;
    float m_armor;

    // Movement properties
    float m_speed;
    float m_jumpForce;
    bool m_isJumping;
    glm::vec3 m_velocity;

    // Weapon management
    Weapon* m_weapons[3];
    Weapon* m_currentWeapon;
    int m_currentWeaponIndex;

    // Collision properties
    float m_height;
    float m_radius;

    // Helper methods
    void applyGravity(float deltaTime);
    void updateCollisionBox();
};
