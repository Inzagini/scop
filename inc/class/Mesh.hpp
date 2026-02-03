#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "glm/glm.hpp"

struct Material
{
   float opacity;
   float shininess;
   glm::vec3 ambient;
   glm::vec3 diffuse;
   glm::vec3 specular;
};

struct ObjProp
{
    int size;
    Material material;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};


class Mesh
{

    public:
        Mesh(const ObjProp &obj, const unsigned int &size, const unsigned int drawType);
        ~Mesh();
        void draw();
        const float getOpacity() const;

    private:
        const float     opacity{1};
        unsigned int    VBO{}, VAO{}, EBO{};
        size_t          indexCount{};
        size_t          vertexCount{};
};