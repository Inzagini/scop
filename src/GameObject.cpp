#include "class/GameObject.hpp"

void GameObject::draw(Shader &shader)
{
    // transform.translateView();
    unsigned int modelLoc = glGetUniformLocation(shader.getID(), "model");
    unsigned int viewLoc  = glGetUniformLocation(shader.getID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &transform.translateView()[0][0]);
    mesh->draw();
}
