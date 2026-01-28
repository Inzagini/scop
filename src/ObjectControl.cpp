#include "class/ObjectControl.hpp"

void ObjectControl::init(GLFWwindow *window, GameObject *object)
{
    this->window = window;
    this->gameObject = object;
}

void ObjectControl::mouseHandler()
{
    mouseRotationHandler(window, gameObject);
    mouseScaleHandler(window, gameObject);
}


void ObjectControl::mouseScaleHandler(GLFWwindow *window, GameObject *object)
{
    glfwSetScrollCallback(window, scrollCallback);
}

void ObjectControl::mouseRotationHandler(GLFWwindow *window, GameObject *object)
{
    bool pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

    if (pressed)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (!middleMousePressed)
        {
            // First frame of press
            lastMouseX = x;
            lastMouseY = y;
            middleMousePressed = true;
        }
        else
        {
            double dx = x - lastMouseX;
            double dy = y - lastMouseY;

            lastMouseX = x;
            lastMouseY = y;

            float sensitivity = 0.01f;

            object->transform.setRotationY(
                object->transform.getRotationY() + dx * sensitivity
            );

            object->transform.setRotationX(
                object->transform.getRotationX() + dy * sensitivity
            );
        }
    }
    else
    {
        middleMousePressed = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}