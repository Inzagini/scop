#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "glm/glm.hpp"

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float opacity;
    float shininess;
};

struct ObjProp
{
    Material material;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    int size;
};


class Mesh
{

    public:
        Mesh(const ObjProp &obj, const unsigned int &size, const unsigned int drawType);
        ~Mesh();
        void draw();
        const float getOpacity() const;

    private:
        size_t          indexCount{};
        size_t          vertexCount{};
        const float     opacity{1};
        unsigned int    VBO{}, VAO{}, EBO{};
};