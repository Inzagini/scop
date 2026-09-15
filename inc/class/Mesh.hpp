#pragma once

#include "MathUtils.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>

struct Material {
  Vec3 ambient = Vec3(0.0f);
  Vec3 diffuse = Vec3(0.8f);
  Vec3 specular = Vec3(1.0f);
  float opacity = 1.0f;
  float shininess = 32.0f;

  Material() = default;
};

struct ObjProp {
  Material material;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  int size;
};

class Mesh {

public:
  Mesh(const ObjProp& obj, const unsigned int& size,
       const unsigned int drawType);
  ~Mesh();
  void draw();
  float getOpacity() const;

private:
  size_t indexCount{};
  size_t vertexCount{};
  const float opacity{1};
  unsigned int VBO{}, VAO{}, EBO{};
};
