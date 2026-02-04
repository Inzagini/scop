#pragma once

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "MathUtils.hpp"

class Transform {

    public:
        glm::mat4   getModel() const;
        glm::mat4   translateView() const;

        void        setPositionX(float n);
        void        setPositionY(float n);
        void        setPositionZ(float n);
        float       getPositionX();
        float       getPositionY();
        float       getPositionZ();

        void        setRotationX(float n);
        void        setRotationY(float n);
        void        setRotationZ(float n);
        float       getRotationX();
        float       getRotationY();
        float       getRotationZ();

        void        setScale(float scaleSpeed);
        glm::vec3   getScale();

    private:
        glm::vec3 position {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation {0.0f, 0.0f, 0.0f};
        glm::vec3 scale {0.5f};
};