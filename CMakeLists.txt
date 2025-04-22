cmake_minimum_required(VERSION 3.10)
project(Ag-n)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/Player.cpp
    src/Renderer.cpp
    src/VertexBuffer.cpp
    src/Weapon.cpp
    src/Level.cpp
)

# Header files
set(HEADERS
    src/Camera.hpp
    src/Mesh.hpp
    src/Player.hpp
    src/Renderer.hpp
    src/VertexBuffer.hpp
    src/Weapon.hpp
    src/Level.hpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw
    GLEW::GLEW
    glm::glm
)

# Copy shader files to build directory
file(COPY res/shaders DESTINATION ${CMAKE_BINARY_DIR}/res)
