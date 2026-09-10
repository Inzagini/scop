cmake_minimum_required(VERSION 3.20)

project(scop LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.cpp
    src/Window.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/Transform.cpp
    src/GameObject.cpp
    src/CameraControl.cpp
    src/parse.cpp
    src/processInput.cpp
    src/MathUtils.cpp
    glad/src/glad.c
)

add_executable(scop ${SOURCES})

target_include_directories(scop PRIVATE
    .
    inc
    glad/include
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(scop PRIVATE
    glfw
    OpenGL::GL
)
