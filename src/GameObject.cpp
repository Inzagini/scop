#include "class/GameObject.hpp"

// Draws the mesh, enabling alpha blending when the material is transparent.
void GameObject::draw() {
  const float solid{1.0f};

  glEnable(GL_DEPTH_TEST);

  if (mesh->getOpacity() < solid) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    mesh->draw();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
  } else
    mesh->draw();
}

// Redraws the mesh as triangle outlines in the shader's wireframe mode.
void GameObject::drawWireframe(Shader& shader) {
  shader.setBool("wireframe", true);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_POLYGON_OFFSET_LINE);
  glPolygonOffset(-1.0f, -1.0f);
  draw();
  glDisable(GL_POLYGON_OFFSET_LINE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  shader.setBool("wireframe", false);
}

Mesh* GameObject::getMesh() { return mesh; }

Transform GameObject::getTransform() { return transform; }

// Handles per-frame input and delegates movement to movementHandler.
void GameObject::inputHandler(GLFWwindow* window, float& dTime) {
  static bool tWasPressed = false;
  const bool tPressed = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;

  if (tPressed && !tWasPressed)
    autoRotate = !autoRotate;

  tWasPressed = tPressed;

  movementHandler(window, dTime);
}

// Recentres the object at the origin.
void GameObject::centerObj() {
  transform.setPositionX(0);
  transform.setPositionY(0);
  transform.setPositionZ(0);
}

// Rotates the object by left-drag mouse movement.
void GameObject::mouseHandler(GLFWwindow* window, float& dTime) {

  double mx, my;
  glfwGetCursorPos(window, &mx, &my);

  bool leftDown =
      glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

  if (leftDown && !mouseDragging) {
    mouseDragging = true;
    lastMouseX = static_cast<float>(mx);
    lastMouseY = static_cast<float>(my);
  } else if (!leftDown) {
    mouseDragging = false;
  } else {
    float dx = static_cast<float>(mx) - lastMouseX;
    float dy = static_cast<float>(my) - lastMouseY;
    lastMouseX = static_cast<float>(mx);
    lastMouseY = static_cast<float>(my);

    const float sensitivity = 0.3f;

    transform.setRotationY(transform.getRotationY() + dx * sensitivity);
    transform.setRotationX(transform.getRotationX() + dy * sensitivity);
  }
}

// Applies keyboard movement/rotation and the optional auto-rotate turntable.
void GameObject::movementHandler(GLFWwindow* window, float& dTime) {
  float speed = 2.0f;
  constexpr float autoRotateSpeed = 45.0f;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    transform.setPositionY(transform.getPositionY() + speed * dTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    transform.setPositionY(transform.getPositionY() - speed * dTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    transform.setPositionX(transform.getPositionX() - speed * dTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    transform.setPositionX(transform.getPositionX() + speed * dTime);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    centerObj();
  }

  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    transform.setRotationY(transform.getRotationY() - speed * dTime * 30);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    transform.setRotationY(transform.getRotationY() + speed * dTime * 30);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    transform.setRotationX(transform.getRotationX() - speed * dTime * 30);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    transform.setRotationX(transform.getRotationX() + speed * dTime * 30);
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    transform.setRotationZ(transform.getRotationZ() - speed * dTime * 30);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    transform.setRotationZ(transform.getRotationZ() + speed * dTime * 30);
  }
  if (autoRotate)
    transform.setRotationY(transform.getRotationY() + autoRotateSpeed * dTime);

  mouseHandler(window, dTime);
}
