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

namespace MathUtils
{
    float radians(const float degree);
    float length(const glm::vec3 &);
    Mat4 translate(glm::mat4 &, glm::vec3 &);
};

