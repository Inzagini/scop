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
        const float rotationSpeed {1.5f};
        double lastMouseX {0.0};
        double lastMouseY {0.0};
        const glm::vec3     target {0, 0, 0};
        const glm::vec3     up {0, 1, 0};
        bool middleMousePressed {false};
        
        Camera *camera = nullptr;
        GLFWwindow *window = nullptr;

        CameraControl() = default;
        ~CameraControl() = default;

        void WASDHandler(float &dTime);
        void mouseHandler(float &dTime);

        static void scrollCallback(GLFWwindow *window, double dx, double dy)
        {
            CameraControl* ctrl = static_cast<CameraControl*>(glfwGetWindowUserPointer(window));
            if (ctrl)
                ctrl->onScroll(dx, dy);
        }
        void onScroll(double dx, double dy)
        {
            float zoomSpeed = 0.3f;
            glm::vec3 offset = camera->getPosition() - target;
            float distance = glm::length(offset);

            float newDistance = distance - float(dy) * zoomSpeed;

            if (newDistance < 0.1f) newDistance = 0.1f;
            if (newDistance > 10.0f) newDistance = 10.0f;

            offset = glm::normalize(offset) * newDistance;
            camera->move(target + offset);
        }

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

        void onMiddleButtonPress(bool state)
        {
            middleMousePressed = state;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }

        bool getMiddleButtonPressState() const
        {
            return middleMousePressed;
        }

        static void scrollDragCallback(GLFWwindow *window, double xPos, double yPos)
        {
            CameraControl* ctrl = static_cast<CameraControl*>(glfwGetWindowUserPointer(window));
            if (ctrl && ctrl->getMiddleButtonPressState())
                ctrl->onDrag(xPos, yPos);
        }

        void onDrag(double xPos, double yPos)
        {
            float dx = xPos - lastMouseX;
            float dy = yPos - lastMouseY;

            lastMouseX = xPos;
            lastMouseY = yPos;
            float sensitivity = 0.01f;

            glm::vec3 offset = camera->getPosition() - target;

            glm::mat4 horizRot = glm::rotate(glm::mat4(1.0f), -dx * sensitivity, up);
            offset = glm::vec3(horizRot * glm::vec4(offset, 1.0f));

            glm::vec3 right = glm::normalize(glm::cross(offset, up));

            glm::mat4 vertRot = glm::rotate(glm::mat4(1.0f), -dy * sensitivity, right);
            offset = glm::vec3(vertRot * glm::vec4(offset, 1.0f));

            camera->move(target + offset);
            
        }
};