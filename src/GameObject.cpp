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

void GameObject::movementHandler(GLFWwindow *window, float &dTime)
{
    float speed = 2.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        transform.setPositionY(transform.getPositionY() + speed * dTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        transform.setPositionY(transform.getPositionY() - speed * dTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        transform.setPositionX(transform.getPositionX() - speed * dTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        transform.setPositionX(transform.getPositionX() + speed * dTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //put object to center
    {
        transform.setPositionX(0);
        transform.setPositionY(0);
        transform.setPositionZ(0);
    }
}