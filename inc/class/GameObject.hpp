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
  void mouseHandler(GLFWwindow* window, float& dTime);

private:
  Mesh* mesh;
  Transform transform;

  bool mouseDragging = false;
  float lastMouseX = 0.0f;
  float lastMouseY = 0.0f;

  bool autoRotate = false; // T-key turntable, off by default
};
