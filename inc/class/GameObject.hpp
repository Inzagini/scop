#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Transform.hpp"

class GameObject {
public:
  GameObject(Mesh& mesh) : mesh(&mesh) {}
  void draw();
  Mesh* getMesh();
  Transform getTransform();
  void inputHandler(GLFWwindow* window, float& dTime);

private:
  void movementHandler(GLFWwindow* window, float& dTime);
  void centerObj();

private:
  Mesh* mesh;
  Transform transform;
};
