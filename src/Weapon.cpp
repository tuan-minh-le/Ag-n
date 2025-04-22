#include "Weapon.hpp"
#include <iostream>
#include <algorithm>

Weapon::Weapon(const std::string& name, float damage, float fireRate, int magazineSize)
    : m_name(name)
    , m_damage(damage)
    , m_fireRate(fireRate)
    , m_magazineSize(magazineSize)
    , m_currentAmmo(magazineSize)
    , m_isReloading(false)
    , m_reloadTime(2.0f)  // Default 2 seconds reload time
    , m_spread(0.1f)      // Default spread
    , m_recoil(0.2f)      // Default recoil
    , m_isAutomatic(true) // Default to automatic
{
    // Set default values based on weapon type
    if (name == "Pistol") {
        m_reloadTime = 1.5f;
        m_spread = 0.05f;
        m_recoil = 0.3f;
        m_isAutomatic = false;
    } else if (name == "Shotgun") {
        m_reloadTime = 3.0f;
        m_spread = 0.3f;
        m_recoil = 0.8f;
        m_isAutomatic = false;
    } else if (name == "SMG") {
        m_reloadTime = 2.5f;
        m_spread = 0.15f;
        m_recoil = 0.4f;
        m_isAutomatic = true;
    }

    m_lastShotTime = std::chrono::steady_clock::now();
}

bool Weapon::shoot() {
    if (!canShoot()) {
        return false;
    }

    m_currentAmmo--;
    m_lastShotTime = std::chrono::steady_clock::now();

    // Here we would trigger shooting effects, sounds, etc.
    return true;
}

void Weapon::reload() {
    if (m_currentAmmo == m_magazineSize || m_isReloading) {
        return;
    }

    m_isReloading = true;
    m_reloadStartTime = std::chrono::steady_clock::now();
}

bool Weapon::canShoot() const {
    if (m_isReloading || m_currentAmmo <= 0) {
        return false;
    }

    // Check fire rate
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastShot = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - m_lastShotTime).count();

    float shotsPerSecond = m_fireRate / 60.0f;
    float timeBetweenShots = 1000.0f / shotsPerSecond; // in milliseconds

    return timeSinceLastShot >= timeBetweenShots;
}

bool Weapon::needsReload() const {
    return m_currentAmmo <= 0 && !m_isReloading;
}

bool Weapon::isReloading() const {
    return m_isReloading;
}

void Weapon::update(float deltaTime) {
    if (m_isReloading) {
        auto now = std::chrono::steady_clock::now();
        auto reloadDuration = std::chrono::duration_cast<std::chrono::seconds>(
            now - m_reloadStartTime).count();

        if (reloadDuration >= m_reloadTime) {
            m_currentAmmo = m_magazineSize;
            m_isReloading = false;
        }
    }
}
