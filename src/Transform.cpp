#include "class/Transform.hpp"

Mat4 Transform::getModel() const
{
    Mat4 model;

    model = MathUtils::translate(model, position);
    model = MathUtils::rotateX(model, MathUtils::radians(rotation.x));
    model = MathUtils::rotateY(model, MathUtils::radians(rotation.y));
    model = MathUtils::rotateZ(model, MathUtils::radians(rotation.z));
    model = MathUtils::scale(model, scale);

    return model;
}

Mat4 Transform::translateView() const
{
    Mat4 view;

    view = MathUtils::translate(view, position);

    return view;
}


void Transform::setPositionX(float n)
{
    position.x = n;
}

void Transform::setPositionY(float n)
{
    position.y = n;
}

void Transform::setPositionZ(float n)
{
    position.z = n;
}

float Transform::getPositionX()
{
    return position.x;
}

float Transform::getPositionY()
{
    return position.y;
}

float Transform::getPositionZ()
{
    return position.z;
}

void Transform::setRotationX(float n)
{
    rotation.x = n;
}

void Transform::setRotationY(float n)
{
    rotation.y = n;
}

void Transform::setRotationZ(float n)
{
    rotation.z = n;
}

float Transform::getRotationX()
{
    return rotation.x;
}

float Transform::getRotationY()
{
    return rotation.y;
}

float Transform::getRotationZ()
{
    return rotation.z;
}

void Transform::setScale(float scaleSpeed)
{
    scale.x = scale.x + scaleSpeed;
    scale.y = scale.y + scaleSpeed;
    scale.z = scale.z + scaleSpeed;
}

Vec3 Transform::getScale()
{
    return scale;
}