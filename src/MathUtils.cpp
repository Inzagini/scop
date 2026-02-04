#include "MathUtils.hpp"

float MathUtils::radians(const float degree)
{
    return degree * (M_PI / 180);
}

float MathUtils::length(const glm::vec3 &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Mat4 MathUtils::translate(glm::mat4 &mat, glm::vec3 &v)
{
    Mat4 res;

    for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                res.m[i][j] = mat[i][j];

    res.m[3][0] += v.x;
    res.m[3][1] += v.y;
    res.m[3][2] += v.z;

    return res;
}
  