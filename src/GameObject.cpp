#include "class/GameObject.hpp"

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

Mesh* GameObject::getMesh() { return mesh; }

Transform GameObject::getTransform() { return transform; }

void GameObject::inputHandler(GLFWwindow* window, float& dTime) {
  // T toggles the auto-rotate turntable. Edge detection so one press flips it
  // exactly once instead of once per frame while the key is held.
  static bool tWasPressed = false;
  const bool tPressed = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;

  if (tPressed && !tWasPressed)
    autoRotate = !autoRotate;

  tWasPressed = tPressed;

  movementHandler(window, dTime);
}

void GameObject::centerObj() {
  transform.setPositionX(0);
  transform.setPositionY(0);
  transform.setPositionZ(0);
}

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

void GameObject::movementHandler(GLFWwindow* window, float& dTime) {
  float speed = 2.0f;
  constexpr float autoRotateSpeed = 45.0f; // turntable spin, degrees per second

  // Move Game object
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

  // Rotation Game object
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
  // Auto-rotate turntable: keep spinning around Y every frame while T is on.
  // Added after the manual keys so it composes with user rotation.
  if (autoRotate)
    transform.setRotationY(transform.getRotationY() + autoRotateSpeed * dTime);

  mouseHandler(window, dTime);
}
