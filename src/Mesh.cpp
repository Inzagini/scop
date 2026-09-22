#include "class/Mesh.hpp"
#include "Texture.hpp"

// Uploads positions, UVs, normals and indices to GPU buffers and loads the
// material's diffuse texture, if any.
Mesh::Mesh(const ObjProp& obj, const unsigned int& size,
           const unsigned int drawType)
    : opacity(obj.material.opacity) {
  indexCount = obj.indices.size();
  vertexCount = obj.vertices.size() / 3;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, obj.vertices.size() * sizeof(float),
               obj.vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  if (!obj.indices.empty()) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 obj.indices.size() * sizeof(unsigned int), obj.indices.data(),
                 drawType);
  }

  if (!obj.texCoords.empty()) {
    glGenBuffers(1, &uvVBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    glBufferData(GL_ARRAY_BUFFER, obj.texCoords.size() * sizeof(float),
                 obj.texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(2);
  }

  glGenBuffers(1, &normalVBO);
  glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
  glBufferData(GL_ARRAY_BUFFER, obj.normals.size() * sizeof(float),
               obj.normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  if (!obj.material.diffuseMap.empty()) {
    std::cerr << "[Mesh] about to load: '" << obj.material.diffuseMap << "'\n";
    texture = loadTexture2D(obj.material.diffuseMap);
    std::cerr << "[Mesh] texture id = " << texture << '\n';
  } else {
    std::cerr << "[Mesh] diffuseMap is EMPTY, skipping texture load\n";
  }
}

// Releases the GL buffers, VAO and texture.
Mesh::~Mesh() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &uvVBO);
  glDeleteBuffers(1, &normalVBO);
  if (texture)
    glDeleteTextures(1, &texture);
}

// Draws the mesh with its index buffer, or the vertex array if unindexed.
void Mesh::draw() {
  glBindVertexArray(VAO);
  if (indexCount)
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  else
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

  glBindVertexArray(0);
}

float Mesh::getOpacity() const { return opacity; }

bool Mesh::hasTexture() const { return texture != 0; }

unsigned int Mesh::getTexture() const { return texture; }
