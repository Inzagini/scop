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

// void GameObject::WASDHandler(float &dTime)
// {
//     float rotationSpeed = 1.5f;
//     float scaleSpeed = 1.0f * dTime;

//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//             glfwSetWindowShouldClose(window, true);    

//     //z axis forward
//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//     {
//         camera->move({0.0f, 0.0f, -0.1f});
//     }

//     //z axis backward
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//     {
//         camera->move({0.0f, 0.0f, 0.1f});
//     }  

//     //x axis left
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//     {
//         camera->move({-0.1f, 0.0f, 0.0f});
//     } 

//     //x axis right
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//     {
//         camera->move({0.1f, 0.0f, 0.0f});
//     }

//     if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//     {
//         camera->move(
//                 {(0.01f), 0.0f,(0.01f)});
//     }

//     if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//     {
//         camera->move({-0.01f, -0.00f, -0.01f});
//     }
// }