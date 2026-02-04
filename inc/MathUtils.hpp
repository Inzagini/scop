#pragma once

#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Mat4
{
    float m[4][4];

    Mat4()
    {
        for (int i{}; i < 4; i++)
        {
            for (int j{}; j < 4; j++)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
};

struct Vec3
{
    float x{}, y{}, z{};

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}

    Vec3(float n) : x(n), y(n), z(n) {}

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

namespace MathUtils
{
    float radians(const float degree);
    float length(const glm::vec3 &);
    Mat4 multiply(const Mat4 &, const Mat4 &);
    // Mat4 translate(Mat4 &, Vec3 &);
    Mat4 translate(Mat4 , Vec3 );
    Mat4 scale(const Mat4 &, const Vec3 &);
    Mat4 rotateX(Mat4, float angle);
    Mat4 rotateY(Mat4, float angle);
    Mat4 rotateZ(Mat4, float angle);
};

