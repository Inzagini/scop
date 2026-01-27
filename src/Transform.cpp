#include "class/Transform.hpp"

glm::mat4 Transform::getModel() const
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 rotateX{1.0f, 0.0f, 0.0f};
    glm::vec3 rotateY{0.0f, 1.0f, 0.0f};
    glm::vec3 rotateZ{0.0f, 0.0f, 1.0f};

    // model = glm::translate(model, glm::vec3{0.0f});
    model = glm::rotate(model, rotation.x, rotateX);
    model = glm::rotate(model, rotation.y, rotateY);
    model = glm::rotate(model, rotation.z, rotateZ);
    model = glm::scale(model, scale);

    return model;
}

glm::mat4 Transform::translateView() const
{
    glm::mat4 view = glm::mat4(0.0f);

    view = translate(view, position);

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
    scale = scale + scaleSpeed;
}

glm::vec3 Transform::getScale()
{
    return scale;
}