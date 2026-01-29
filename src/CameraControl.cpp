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
    WASDHandler(dTime);
}


void CameraControl::WASDHandler(float &dTime)
{
    float rotationSpeed = 1.5f;
    float scaleSpeed = 1.0f * dTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);    

    //z axis forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->move({0.0f, 0.0f, -0.1f});
    }

    //z axis backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->move({0.0f, 0.0f, 0.1f});
    }  

    //x axis left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera->move({-0.1f, 0.0f, 0.0f});
    } 

    //x axis right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera->move({0.1f, 0.0f, 0.0f});
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera->move(
                {(0.01f), 0.0f,(0.01f)});
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera->move({-0.01f, -0.00f, -0.01f});
    }
}

void CameraControl::mouseHandler(float &dTime)
{
    //handling the scrollwheel
    glfwSetScrollCallback(window, scrollCallback);

    //handling the scrollwheel click drag
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, scrollDragCallback);
}
