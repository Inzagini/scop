#pragma once

#include "Window.hpp"
#include <iostream>
#include "MathUtils.hpp"

class Camera
{
    public:
        Mat4   getView(void) const
        {
            return MathUtils::lookAt(position, target, up);
        }
        Mat4    getProjection(void) const{return MathUtils::perspective(MathUtils::radians(fov), aspectRatio, nearPlane, farPlane);}
        Vec3    getPosition(void) const {return position;}
        void    move(const Vec3 n) {position = n;}

        float   getYaw() {return yaw;}
        void    setYaw(float n) {yaw = n;}

        float   getPitch() {return pitch;}
        void    setPitch(float n) {pitch = n;}

        float   getRadius() {return radius;}
        void    setRadius(float n) {radius = n;}

    private:
        Vec3            position {0.0f, 1.0f, 3.0f};
        const Vec3      target {0, 0, 0};
        const Vec3      up {0, 1, 0};
        const float     fov{45.0f};
        const float     aspectRatio = static_cast<float>(SCR_WIDTH) / SCR_HEIGHT;
        const float     nearPlane{0.1f};
        const float     farPlane{100.0f};
        float           radius = MathUtils::length(position - target); //distance camera from target
        float           yaw = MathUtils::degrees(atan2((position - target).z, (position - target).x));
        float           pitch = MathUtils::degrees(asin((position - target).y / radius));

};
