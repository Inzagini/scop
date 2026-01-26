#pragma once

#include "Window.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera
{
    public:
        glm::mat4 getView(void) const
        {
            return glm::translate(glm::mat4(1.0f), -positon);
        }

        glm::mat4   getProjection(void) const
        {
            return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
        }

    private:
        const glm::vec3     positon {0.0f, 0.0f, 3.0f};
        const glm::vec3     prespective {1.0f};
        const float         fov{45.0f};
        const float         aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        const float         nearPlane{0.1f};
        const float         farPlane{100.0f};
};