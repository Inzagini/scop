#pragma once

#include "Window.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

class Camera
{
    public:

        glm::mat4 getView(void) const
        {
            return glm::lookAt(position, target, up);
        }

        glm::mat4   getProjection(void) const
        {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        }

        glm::vec3 getPosition(void) const
        {
            return position;
        }

        void move(const glm::vec3 n)
        {
            position = n;
        }

    private:
        glm::vec3           position {0.0f, 1.0f, 3.0f};
        const glm::vec3     target {0, 0, 0};
        const glm::vec3     up {0, 1, 0};
        const float         fov{45.0f};
        const float         aspectRatio = static_cast<float>(SCR_WIDTH) / SCR_HEIGHT;
        const float         nearPlane{0.1f};
        const float         farPlane{100.0f};

};