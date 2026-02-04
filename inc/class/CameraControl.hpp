#pragma once

#include "class/Camera.hpp"

class CameraControl
{
    public:
        CameraControl(const CameraControl&) = delete;
        CameraControl& operator=(const CameraControl&) = delete;

        void init(GLFWwindow *window, Camera *camera);

        static CameraControl &getInstance()
        {
            static CameraControl instance;
            return instance;
        }

        void movementHandler(float &n);
        

    private:
        const glm::vec3     target {0, 0, 0};
        const glm::vec3     up {0, 1, 0};
        double lastMouseX {0.0};
        double lastMouseY {0.0};

        Camera *camera = nullptr;
        GLFWwindow *window = nullptr;

        const float rotationSpeed {1.5f};
        bool middleMousePressed {false};

        CameraControl() = default;
        ~CameraControl() = default;

        void mouseHandler(float &dTime);

        static void scrollCallback(GLFWwindow *window, double dx, double dy)
        {
            CameraControl* ctrl = static_cast<CameraControl*>(glfwGetWindowUserPointer(window));
            if (ctrl)
                ctrl->onScroll(dx, dy);
        }
        void onScroll(double dx, double dy);

        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
            
            CameraControl* ctrl = static_cast<CameraControl*>(glfwGetWindowUserPointer(window));
            
            if (button == GLFW_MOUSE_BUTTON_MIDDLE)
            {
                if (action == GLFW_PRESS && ctrl)
                {
                    ctrl->onMiddleButtonPress(true);
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                } else if (action == GLFW_RELEASE && ctrl)
                {
                    ctrl->onMiddleButtonPress(false);
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
            }
        }

        void onMiddleButtonPress(bool state);

        bool getMiddleButtonPressState() const;

        static void scrollDragCallback(GLFWwindow *window, double xPos, double yPos)
        {
            CameraControl* ctrl = static_cast<CameraControl*>(glfwGetWindowUserPointer(window));
            if (ctrl && ctrl->getMiddleButtonPressState())
                ctrl->onDrag(xPos, yPos);
        }

        void onDrag(double xPos, double yPos);
};