#pragma once

#include <string>
#include <chrono>

class Weapon {
public:
    Weapon(const std::string& name, float damage, float fireRate, int magazineSize);

    // Weapon actions
    bool shoot();
    void reload();

    // State queries
    bool canShoot() const;
    bool needsReload() const;
    bool isReloading() const;

    // Getters
    const std::string& getName() const { return m_name; }
    float getDamage() const { return m_damage; }
    int getAmmoCount() const { return m_currentAmmo; }
    int getMagazineSize() const { return m_magazineSize; }

    // Update (for animations, reload timing, etc.)
    void update(float deltaTime);

private:
    std::string m_name;
    float m_damage;
    float m_fireRate;      // Rounds per minute
    int m_magazineSize;
    int m_currentAmmo;

    // Timing
    std::chrono::steady_clock::time_point m_lastShotTime;
    std::chrono::steady_clock::time_point m_reloadStartTime;
    bool m_isReloading;
    float m_reloadTime;    // Time to reload in seconds

    // Weapon characteristics
    float m_spread;        // Accuracy spread
    float m_recoil;        // Recoil amount
    bool m_isAutomatic;    // Whether the weapon fires automatically
};  
