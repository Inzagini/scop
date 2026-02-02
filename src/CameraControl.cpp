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
