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
  movementHandler(window, dTime);
}

void GameObject::centerObj() {
  transform.setPositionX(0);
  transform.setPositionY(0);
  transform.setPositionZ(0);
}

void GameObject::movementHandler(GLFWwindow* window, float& dTime) {
  float speed = 2.0f;

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
    transform.setRotationY(transform.getRotationY() - speed * dTime * 10);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    transform.setRotationY(transform.getRotationY() + speed * dTime * 10);
  }
}
