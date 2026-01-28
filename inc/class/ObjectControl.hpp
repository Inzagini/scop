#pragma once

#include "class/GameObject.hpp"

class ObjectControl
{
    public:
        ObjectControl(const ObjectControl&) = delete;
        ObjectControl& operator=(const ObjectControl&) = delete;

        void init(GLFWwindow *window, GameObject *gameObject);

        static ObjectControl &getInstance()
        {
            static ObjectControl instance;
            return instance;
        }

        void mouseHandler();

    private:
        const float rotationSpeed = 1.5f;
        float scaleSpeed = 1.0f;
        double lastMouseX = 0.0;
        double lastMouseY = 0.0;
        bool middleMousePressed = false;
        GameObject *gameObject = nullptr;
        GLFWwindow *window = nullptr;

        ObjectControl() = default;
        ~ObjectControl() = default;

        void mouseRotationHandler(GLFWwindow *window, GameObject *object);
        void mouseScaleHandler(GLFWwindow *window, GameObject *object);
        static void scrollCallback(GLFWwindow *window, double dx, double dy)
        {
            ObjectControl* ctrl = static_cast<ObjectControl*>(glfwGetWindowUserPointer(window));
            if (ctrl)
                ctrl->onScroll(dx, dy);
        }
        void onScroll(double dx, double dy)
        {
            scaleSpeed = dy * 0.02f;
            gameObject->transform.setScale(scaleSpeed);
        }

 
};