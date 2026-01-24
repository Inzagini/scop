#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>

class Mesh
{

    public:
        Mesh(const std::vector<float> &vertices, const unsigned int &size, const unsigned int drawType,const std::vector<unsigned int> &indices = {});
        ~Mesh();
        void draw();
        void processInput(GLFWwindow *window);

    private:
        unsigned int    VBO{}, VAO{}, EBO{};
        size_t          indexCount{};
        size_t          vertexCount{};
        float           rotationX = 0.0f;
        float           rotationY = 0.0f;
        float           rotationSpeed = 1.5f;
        float           deltaTime = 0.0f;
        float           lastFrame = 0.0f;
};