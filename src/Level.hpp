#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Mesh.hpp"

class Level {
public:
    Level();
    ~Level();

    // Level generation
    void generateApartment();

    // Collision detection
    bool checkCollision(const glm::vec3& position, float radius) const;

    // Rendering
    const std::vector<Mesh*>& getMeshes() const { return m_meshes; }

    // Cover system
    bool checkCoverPosition(const glm::vec3& position) const;
    std::vector<glm::vec3> getNearestCoverPositions(const glm::vec3& position, float radius) const;

private:
    struct Room {
        glm::vec3 position;
        glm::vec3 size;
        std::string type; // "kitchen", "bedroom", "living_room", etc.
        std::vector<glm::vec3> coverPositions;
    };

    struct Wall {
        glm::vec3 start;
        glm::vec3 end;
        float height;
        bool hasDoor;
        glm::vec3 doorPosition;
        float doorWidth;
    };

    std::vector<Mesh*> m_meshes;
    std::vector<Room> m_rooms;
    std::vector<Wall> m_walls;

    // Helper methods
    void createRoom(const glm::vec3& position, const glm::vec3& size, const std::string& type);
    void createWall(const glm::vec3& start, const glm::vec3& end, float height, bool hasDoor = false);
    void addFurniture(const Room& room);
    void createDoor(const glm::vec3& position, float width, float height, bool isVertical);
    void generateCoverPositions();
};
