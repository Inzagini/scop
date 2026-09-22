#include "class/CameraControl.hpp"

#include <iostream>

void CameraControl::init(GLFWwindow* window, Camera* camera) {
  this->window = window;
  this->camera = camera;
  maxZoomOut = camera->getRadius() * 4.0f; // zoom-out headroom past the fitted distance
}

void CameraControl::movementHandler() { mouseHandler(); }

void CameraControl::mouseHandler() {
  // handling the scrollwheel
  glfwSetScrollCallback(window, scrollCallback);

  // handling the scrollwheel click drag
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetCursorPosCallback(window, scrollDragCallback);
}

void CameraControl::onScroll(double dx, double dy) {
  dx = dx; // not used
  Vec3 offset = camera->getPosition() - target;

  // Holding Ctrl (either side) makes each scroll step ten times faster.
  const bool fastZoom =
      glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
  const float zoomSpeed = fastZoom ? 3.0f : 0.3f; // 10x the normal 0.3 step

  float distance = MathUtils::length(offset);

  float newDistance = distance - float(dy) * zoomSpeed;

  newDistance = newDistance < maxZoomIn ? maxZoomIn : newDistance;
  newDistance = newDistance > maxZoomOut ? maxZoomOut : newDistance;

  offset = MathUtils::normalize(offset) * newDistance;
  camera->setRadius(MathUtils::length(offset));
  camera->move(target + offset);
}

void CameraControl::onDrag(double xPos, double yPos) {
  float dx = xPos - lastMouseX;
  float dy = yPos - lastMouseY;

  lastMouseX = xPos;
  lastMouseY = yPos;

  constexpr float sensitivity = 0.1f;
  dx *= sensitivity;
  dy *= sensitivity;

  float newYaw = camera->getYaw() + dx;
  float newPitch = camera->getPitch() + dy;

  newPitch = newPitch > 89.0f ? 89.0f : newPitch;
  newPitch = newPitch < -89.0f ? -89.0f : newPitch;

  camera->setYaw(newYaw);
  camera->setPitch(newPitch);

  Vec3 direction;
  direction.x = camera->getRadius() * cos(MathUtils::radians(newYaw)) *
                cos(MathUtils::radians(newPitch));
  direction.y = camera->getRadius() * sin(MathUtils::radians(newPitch));
  direction.z = camera->getRadius() * sin(MathUtils::radians(newYaw)) *
                cos(MathUtils::radians(newPitch));

  camera->move(target + direction);
}

void CameraControl::onMiddleButtonPress(bool state) {
  middleMousePressed = state;
  glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
}

bool CameraControl::getMiddleButtonPressState() const {
  return middleMousePressed;
}
