#pragma once

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "MathUtils.hpp"

class Transform {

    public:
        Mat4   getModel() const;
        Mat4   translateView() const;

        void   setPositionX(float n);
        void   setPositionY(float n);
        void   setPositionZ(float n);
        float  getPositionX();
        float  getPositionY();
        float  getPositionZ();
        void   setRotationX(float n);
        void   setRotationY(float n);
        void   setRotationZ(float n);
        float  getRotationX();
        float  getRotationY();
        float  getRotationZ();

        void   setScale(float scaleSpeed);
        Vec3   getScale();

    private:
        Vec3 position {0.0f, 0.0f, 0.0f};
        Vec3 rotation {0.0f, 0.0f, 0.0f};
        Vec3 scale {0.5f};
};