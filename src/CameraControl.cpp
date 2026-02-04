#include "class/CameraControl.hpp"

#include <iostream>

void CameraControl::init(GLFWwindow *window, Camera *camera)
{
    this->window = window;
    this->camera = camera;
}

void CameraControl::movementHandler(float &dTime)
{
    mouseHandler(dTime);
}

void CameraControl::mouseHandler(float &dTime)
{
    //handling the scrollwheel
    glfwSetScrollCallback(window, scrollCallback);

    //handling the scrollwheel click drag
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, scrollDragCallback);
}

void CameraControl::onScroll(double dx, double dy)
{
    Vec3 offset = camera->getPosition() - target;
    constexpr float zoomSpeed = 0.3f;
    float distance = MathUtils::length(offset);

    float newDistance = distance - float(dy) * zoomSpeed;

    if (newDistance < 0.1f) newDistance = 0.1f;
    if (newDistance > 10.0f) newDistance = 10.0f;

    offset = MathUtils::normalize(offset) * newDistance;
    camera->setRadius(MathUtils::length(offset));
    camera->move(target + offset);
}

void CameraControl::onDrag(double xPos, double yPos)
{
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
    direction.x = camera->getRadius() * cos(MathUtils::radians(newYaw)) * cos(MathUtils::radians(newPitch));
    direction.y = camera->getRadius() * sin(MathUtils::radians(newPitch));
    direction.z = camera->getRadius() * sin(MathUtils::radians(newYaw)) * cos(MathUtils::radians(newPitch));

    camera->move(target + direction);
}

void CameraControl::onMiddleButtonPress(bool state)
{
    middleMousePressed = state;
    glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
}

bool CameraControl::getMiddleButtonPressState() const
{
    return middleMousePressed;
}