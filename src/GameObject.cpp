#include "class/GameObject.hpp"

void GameObject::draw(Shader &shader)
{
    const float solid {1.0f};

    glEnable(GL_DEPTH_TEST);

    if (mesh->getOpacity() < solid)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
        mesh->draw();
        glDepthMask(GL_TRUE); 
        glDisable(GL_BLEND);
    }
    else
        mesh->draw();
}

Mesh *GameObject::getMesh()
{
    return mesh;
}

Transform GameObject::getTransform()
{
    return transform;
}
