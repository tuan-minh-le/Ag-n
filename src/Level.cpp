#include "Level.hpp"
#include <iostream>

Level::Level() {
    generateApartment();
}

Level::~Level() {
    for (auto mesh : m_meshes) {
        delete mesh;
    }
}

void Level::generateApartment() {
    // Clear existing geometry
    for (auto mesh : m_meshes) {
        delete mesh;
    }
    m_meshes.clear();
    m_rooms.clear();
    m_walls.clear();

    // Create apartment layout - a typical 1-bedroom apartment
    float wallHeight = 2.8f;

    // Living room - largest room
    createRoom(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(6.0f, wallHeight, 8.0f), "living_room");

    // Kitchen - connected to living room
    createRoom(glm::vec3(-3.0f, 0.0f, -8.0f), glm::vec3(4.0f, wallHeight, 4.0f), "kitchen");

    // Bedroom
    createRoom(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(4.0f, wallHeight, 6.0f), "bedroom");

    // Bathroom
    createRoom(glm::vec3(6.0f, 0.0f, -4.0f), glm::vec3(3.0f, wallHeight, 3.0f), "bathroom");

    // Hallway/corridor
    createRoom(glm::vec3(3.0f, 0.0f, -4.0f), glm::vec3(3.0f, wallHeight, 2.0f), "hallway");

    // Create exterior walls
    float exteriorWidth = 15.0f;
    float exteriorDepth = 15.0f;

    // North wall
    createWall(glm::vec3(-exteriorWidth/2, 0.0f, exteriorDepth/2),
               glm::vec3(exteriorWidth/2, 0.0f, exteriorDepth/2), wallHeight);

    // East wall
    createWall(glm::vec3(exteriorWidth/2, 0.0f, exteriorDepth/2),
               glm::vec3(exteriorWidth/2, 0.0f, -exteriorDepth/2), wallHeight);

    // South wall with main entrance
    createWall(glm::vec3(exteriorWidth/2, 0.0f, -exteriorDepth/2),
               glm::vec3(-exteriorWidth/2, 0.0f, -exteriorDepth/2), wallHeight, true);

    // West wall
    createWall(glm::vec3(-exteriorWidth/2, 0.0f, -exteriorDepth/2),
               glm::vec3(-exteriorWidth/2, 0.0f, exteriorDepth/2), wallHeight);

    // Interior walls and doors
    // Wall between living room and bedroom
    createWall(glm::vec3(3.0f, 0.0f, 4.0f), glm::vec3(3.0f, 0.0f, -2.0f), wallHeight, true);

    // Wall between hallway and kitchen
    createWall(glm::vec3(1.5f, 0.0f, -4.0f), glm::vec3(-1.0f, 0.0f, -4.0f), wallHeight, true);

    // Generate furniture for each room
    for (const auto& room : m_rooms) {
        addFurniture(room);
    }

    // Generate cover positions
    generateCoverPositions();
}

void Level::createRoom(const glm::vec3& position, const glm::vec3& size, const std::string& type) {
    Room room;
    room.position = position;
    room.size = size;
    room.type = type;

    // Create floor mesh
    Mesh* floor = new Mesh();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Floor quad
    Vertex v1, v2, v3, v4;
    v1.position = position;
    v2.position = position + glm::vec3(size.x, 0.0f, 0.0f);
    v3.position = position + glm::vec3(size.x, 0.0f, size.z);
    v4.position = position + glm::vec3(0.0f, 0.0f, size.z);

    // Add texture coordinates and normals
    v1.texCoords = glm::vec2(0.0f, 0.0f);
    v2.texCoords = glm::vec2(1.0f, 0.0f);
    v3.texCoords = glm::vec2(1.0f, 1.0f);
    v4.texCoords = glm::vec2(0.0f, 1.0f);

    v1.normal = v2.normal = v3.normal = v4.normal = glm::vec3(0.0f, 1.0f, 0.0f);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    floor->setVertices(vertices);
    floor->setIndices(indices);
    m_meshes.push_back(floor);

    // Create ceiling mesh (similar to floor but flipped)
    // ... (similar code for ceiling)

    m_rooms.push_back(room);
}

void Level::createWall(const glm::vec3& start, const glm::vec3& end, float height, bool hasDoor) {
    Wall wall;
    wall.start = start;
    wall.end = end;
    wall.height = height;
    wall.hasDoor = hasDoor;

    if (hasDoor) {
        // Position door in the middle of the wall
        wall.doorPosition = (start + end) * 0.5f;
        wall.doorWidth = 0.9f; // Standard door width
    }

    // Create wall mesh
    Mesh* wallMesh = new Mesh();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Calculate wall orientation
    glm::vec3 wallDir = glm::normalize(end - start);
    glm::vec3 wallNormal = glm::vec3(-wallDir.z, 0.0f, wallDir.x);

    // Create wall vertices
    if (!hasDoor) {
        // Simple wall without door
        Vertex v1, v2, v3, v4;
        v1.position = start;
        v2.position = start + glm::vec3(0.0f, height, 0.0f);
        v3.position = end + glm::vec3(0.0f, height, 0.0f);
        v4.position = end;

        v1.normal = v2.normal = v3.normal = v4.normal = wallNormal;

        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        vertices.push_back(v4);

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);
    } else {
        // Wall with door - create segments around door
        createDoor(wall.doorPosition, wall.doorWidth, 2.1f, false);
    }

    wallMesh->setVertices(vertices);
    wallMesh->setIndices(indices);
    m_meshes.push_back(wallMesh);

    m_walls.push_back(wall);
}

void Level::addFurniture(const Room& room) {
    // Add furniture based on room type
    if (room.type == "living_room") {
        // Add sofa, coffee table, TV stand
        // Create simple box meshes for furniture
        // ... implementation for furniture creation
    } else if (room.type == "kitchen") {
        // Add counters, stove, refrigerator
        // ... implementation for kitchen furniture
    } else if (room.type == "bedroom") {
        // Add bed, wardrobe, bedside tables
        // ... implementation for bedroom furniture
    } else if (room.type == "bathroom") {
        // Add toilet, sink, shower
        // ... implementation for bathroom fixtures
    }
}

void Level::createDoor(const glm::vec3& position, float width, float height, bool isVertical) {
    // Create a door frame mesh
    Mesh* doorFrameMesh = new Mesh();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Calculate door orientation
    glm::vec3 doorDirection;
    if (isVertical) {
        doorDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    } else {
        doorDirection = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::vec3 doorNormal = glm::vec3(-doorDirection.z, 0.0f, doorDirection.x);

    // Create the door frame (walls around the door opening)
    // Left and right sides
    float frameWidth = 0.1f;

    // Left frame
    Vertex v1, v2, v3, v4;
    v1.position = position - doorDirection * (width/2 + frameWidth);
    v2.position = v1.position + glm::vec3(0.0f, height, 0.0f);
    v3.position = position - doorDirection * (width/2);
    v4.position = v3.position + glm::vec3(0.0f, height, 0.0f);

    v1.normal = v2.normal = v3.normal = v4.normal = doorNormal;
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    // Right frame
    Vertex v5, v6, v7, v8;
    v5.position = position + doorDirection * (width/2);
    v6.position = v5.position + glm::vec3(0.0f, height, 0.0f);
    v7.position = position + doorDirection * (width/2 + frameWidth);
    v8.position = v7.position + glm::vec3(0.0f, height, 0.0f);

    v5.normal = v6.normal = v7.normal = v8.normal = doorNormal;
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);
    vertices.push_back(v8);

    // Top frame
    Vertex v9, v10, v11, v12;
    v9.position = position - doorDirection * (width/2) + glm::vec3(0.0f, height, 0.0f);
    v10.position = position + doorDirection * (width/2) + glm::vec3(0.0f, height, 0.0f);
    v11.position = v9.position + glm::vec3(0.0f, frameWidth, 0.0f);
    v12.position = v10.position + glm::vec3(0.0f, frameWidth, 0.0f);

    v9.normal = v10.normal = v11.normal = v12.normal = doorNormal;
    vertices.push_back(v9);
    vertices.push_back(v10);
    vertices.push_back(v11);
    vertices.push_back(v12);

    // Create indices for all frame pieces
    // Left frame
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(2); indices.push_back(1); indices.push_back(3);

    // Right frame
    indices.push_back(4); indices.push_back(5); indices.push_back(6);
    indices.push_back(6); indices.push_back(5); indices.push_back(7);

    // Top frame
    indices.push_back(8); indices.push_back(9); indices.push_back(10);
    indices.push_back(10); indices.push_back(9); indices.push_back(11);

    doorFrameMesh->setVertices(vertices);
    doorFrameMesh->setIndices(indices);
    m_meshes.push_back(doorFrameMesh);
}

void Level::generateCoverPositions() {
    // Generate strategic cover positions throughout the apartment
    for (auto& room : m_rooms) {
        if (room.type == "living_room") {
            // Behind sofa
            room.coverPositions.push_back(room.position + glm::vec3(2.0f, 0.0f, 3.0f));
            // Behind TV stand
            room.coverPositions.push_back(room.position + glm::vec3(4.0f, 0.0f, 1.0f));
        } else if (room.type == "kitchen") {
            // Behind counter
            room.coverPositions.push_back(room.position + glm::vec3(1.0f, 0.0f, 2.0f));
            // Behind island
            room.coverPositions.push_back(room.position + glm::vec3(2.0f, 0.0f, 1.0f));
        }
        // Add more cover positions for other room types
    }
}

bool Level::checkCollision(const glm::vec3& position, float radius) const {
    // Check collision with walls
    for (const auto& wall : m_walls) {
        // Basic wall collision detection
        // Would implement proper collision detection algorithm here
        float dist = glm::distance(position, wall.start);
        if (dist < radius) {
            return true;
        }
    }

    return false;
}

bool Level::checkCoverPosition(const glm::vec3& position) const {
    // Check if position is near a cover position
    for (const auto& room : m_rooms) {
        for (const auto& coverPos : room.coverPositions) {
            if (glm::distance(position, coverPos) < 0.5f) {
                return true;
            }
        }
    }
    return false;
}

std::vector<glm::vec3> Level::getNearestCoverPositions(const glm::vec3& position, float radius) const {
    std::vector<glm::vec3> nearCover;

    for (const auto& room : m_rooms) {
        for (const auto& coverPos : room.coverPositions) {
            if (glm::distance(position, coverPos) < radius) {
                nearCover.push_back(coverPos);
            }
        }
    }

    return nearCover;
}
