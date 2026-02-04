#pragma once

#include "Mesh.hpp"
#include "Transform.hpp"
#include "Shader.hpp"

class GameObject
{
    public:
        GameObject(Mesh &mesh) : mesh(&mesh) {}
        void    draw(Shader &shader);
        Mesh    *getMesh();
        Transform getTransform();
        void      inputHandler(GLFWwindow *window, float &dTime);
    
    private:
        void movementHandler(GLFWwindow *window, float &dTime);

    private:
        Mesh        *mesh;
        Transform   transform;
};