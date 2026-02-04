#include "MathUtils.hpp"

float MathUtils::radians(const float degree)
{
    return degree * (M_PI / 180);
}

float MathUtils::degrees(const float rad)
{
    return rad * (180 / M_PI);
}

float MathUtils::length(const glm::vec3 &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float MathUtils::length(const Vec3 &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Mat4 MathUtils::translate(Mat4 &mat, Vec3 &v)
// {
//     Mat4 res;

//     res.m[3][0] = mat.m[0][0] * v.x + mat.m[1][0] * v.y + mat.m[2][0] * v.z + mat.m[3][0];
//     res.m[3][1] = mat.m[0][1] * v.x + mat.m[1][1] * v.y + mat.m[2][1] * v.z + mat.m[3][1];
//     res.m[3][2] = mat.m[0][2] * v.x + mat.m[1][2] * v.y + mat.m[2][2] * v.z + mat.m[3][2];
//     res.m[3][3] = mat.m[3][3];

//     return res;
// }

Mat4 MathUtils::translate(Mat4 mat, Vec3 v)
{
    Mat4 res;

    res.m[3][0] = mat.m[0][0] * v.x + mat.m[1][0] * v.y + mat.m[2][0] * v.z + mat.m[3][0];
    res.m[3][1] = mat.m[0][1] * v.x + mat.m[1][1] * v.y + mat.m[2][1] * v.z + mat.m[3][1];
    res.m[3][2] = mat.m[0][2] * v.x + mat.m[1][2] * v.y + mat.m[2][2] * v.z + mat.m[3][2];
    res.m[3][3] = mat.m[3][3];

    return res;
}

Mat4 MathUtils::multiply(const Mat4& a, const Mat4& b)
{
    Mat4 r;

    for(int c = 0; c < 4; ++c)
    {
        for(int r_ = 0; r_ < 4; ++r_)
        {
            r.m[c][r_] =
                a.m[0][r_] * b.m[c][0] +
                a.m[1][r_] * b.m[c][1] +
                a.m[2][r_] * b.m[c][2] +
                a.m[3][r_] * b.m[c][3];
        }
    }
    return r;
}


Mat4 MathUtils::rotateX(Mat4 mat, float angleRad)
{
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    Mat4 r;
    r.m[1][1] = c ; r.m[2][1] = -s;
    r.m[1][2] = s ; r.m[2][2] = c;

    return multiply(mat, r);
}

Mat4 MathUtils::rotateY(Mat4 mat, float angleRad)
{
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    Mat4 r;
    r.m[0][0] = c ; r.m[2][0] = s;
    r.m[0][2] = -s ; r.m[2][2] = c;

    return multiply(mat, r);
}

Mat4 MathUtils::rotateZ(Mat4 mat, float angleRad)
{
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    Mat4 r;
    r.m[0][0] = c ; r.m[1][0] = -s;
    r.m[0][1] = s ; r.m[1][1] = c;

    return multiply(mat, r);
}

Mat4 MathUtils::scale(const Mat4 &mat, const Vec3 &v)
{
    Mat4 r;

    r.m[0][0] *= v.x;
    r.m[1][1] *= v.y;
    r.m[2][2] *= v.z;

    return multiply(r, mat);
}

float MathUtils::dot(const Vec3 &a, const Vec3 &b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

Vec3 MathUtils::cross(const Vec3 &a, const Vec3 &b)
{
    return
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

Vec3 operator-(const Vec3 &a, const Vec3 &b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 operator+(const Vec3 &a, const Vec3 &b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 operator*(const Vec3 &a, const float k)
{
    return {a.x * k, a.y * k, a.z *k};
}

Vec3 MathUtils::normalize(const Vec3& v)
{
    float len = length(v);

    if (len == 0.0f)
        return { 0.0f, 0.0f, 0.0f };

    return {
        v.x / len,
        v.y / len,
        v.z / len
    };
}


Mat4 MathUtils::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
    Vec3 f = normalize(center - eye);
    Vec3 s = normalize(cross(f, up));
    Vec3 u = cross(s, f);

    Mat4 res;

    res.m[0][0] =  s.x;
    res.m[1][0] =  s.y;
    res.m[2][0] =  s.z;

    res.m[0][1] =  u.x;
    res.m[1][1] =  u.y;
    res.m[2][1] =  u.z;

    res.m[0][2] = -f.x;
    res.m[1][2] = -f.y;
    res.m[2][2] = -f.z;

    res.m[3][0] = -dot(s, eye);
    res.m[3][1] = -dot(u, eye);
    res.m[3][2] =  dot(f, eye);

    return res;
}

Mat4 MathUtils::perspective(const float &fov, const float &aspect, const float &nearPlane, const float &farPlane)
{
    Mat4 res;

    const float f = 1.0f / std::tan(fov / 2.0f);

    res.m[0][0] = f / aspect;
    res.m[1][1] = f;
    res.m[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
    res.m[2][3] = -1.0f;
    res.m[3][2] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
    res.m[3][3] = 0.0f;

    return res;
}