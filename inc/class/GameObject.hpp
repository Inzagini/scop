#pragma once

#include "Mesh.hpp"
#include "Transform.hpp"
#include "Shader.hpp"

class GameObject
{
    public:
        Mesh        *mesh;
        Transform   transform;
        GameObject(Mesh &mesh) : mesh(&mesh) {}
        void    draw(Shader &shader);
};