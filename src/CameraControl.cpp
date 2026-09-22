#include "class/CameraControl.hpp"

#include <iostream>

// Stores the window/camera and the zoom limits.
void CameraControl::init(GLFWwindow* window, Camera* camera) {
  this->window = window;
  this->camera = camera;
  maxZoomOut = camera->getRadius() * 4.0f;
}

// Per-frame camera update hook.
void CameraControl::movementHandler() { mouseHandler(); }

// Registers the scroll and middle-drag input callbacks.
void CameraControl::mouseHandler() {
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetCursorPosCallback(window, scrollDragCallback);
}

// Zooms the camera by the scroll amount (10x faster while Ctrl is held).
void CameraControl::onScroll(double dx, double dy) {
  dx = dx;
  Vec3 offset = camera->getPosition() - target;

  const bool fastZoom =
      glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
  const float zoomSpeed = fastZoom ? 3.0f : 0.3f;

  float distance = MathUtils::length(offset);

  float newDistance = distance - float(dy) * zoomSpeed;

  newDistance = newDistance < maxZoomIn ? maxZoomIn : newDistance;
  newDistance = newDistance > maxZoomOut ? maxZoomOut : newDistance;

  offset = MathUtils::normalize(offset) * newDistance;
  camera->setRadius(MathUtils::length(offset));
  camera->move(target + offset);
}

// Orbits the camera around the target from the middle-drag delta.
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

// Records the middle-button state and seeds the drag anchor.
void CameraControl::onMiddleButtonPress(bool state) {
  middleMousePressed = state;
  glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
}

bool CameraControl::getMiddleButtonPressState() const {
  return middleMousePressed;
}
