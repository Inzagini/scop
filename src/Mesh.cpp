#include "class/Mesh.hpp"

#include <iostream>

Mesh::Mesh(const std::vector<float> &vertices, const unsigned int &size, const unsigned int drawType, const std::vector<unsigned int> &indices)
{
    indexCount = indices.size();
    vertexCount = vertices.size() / 3;
    std::cout << "Vertex count: " << vertexCount << " Index count: " << indexCount << '\n';

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    if (!indices.empty()) glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW); 

    if (!indices.empty())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), drawType);
    }


    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //color attrubibute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // glBindVertexArray(0);

}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}

void Mesh::draw()
{
    glBindVertexArray(VAO);
    if (indexCount)
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    glBindVertexArray(0);
}