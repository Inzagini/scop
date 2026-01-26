#include "scop.hpp"
#include "class/Window.hpp"
#include "class/Shader.hpp"
#include "class/Mesh.hpp"
#include "class/GameObject.hpp"
#include "class/Camera.hpp"

#include <vector>
#include <cmath>


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


// float rotationSpeed = 1.5f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window, GameObject *object)
{
    float rotationSpeed = 1.5f;
    float scaleSpeed = 1.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        object->transform.setRotationX(object->transform.getRotationX() - (rotationSpeed * deltaTime));

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        object->transform.setRotationX(object->transform.getRotationX() + (rotationSpeed * deltaTime));

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        object->transform.setRotationY(object->transform.getRotationY() - (rotationSpeed * deltaTime));

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        object->transform.setRotationY(object->transform.getRotationY() + (rotationSpeed * deltaTime));
    

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        object->transform.setScale(object->transform.getScale() + scaleSpeed);
    
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        object->transform.setScale(object->transform.getScale() - scaleSpeed);
}

int main(int arc, char *argv[])
{
    Window window;
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window.get(), &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    //veritces
    std::vector<float> vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    std::vector<float> a42 = {
         0.232406f, -1.216630f, 1.133818f,
         0.232406f, -0.745504f, 2.843098f,
        -0.227475f, -0.745504f, 2.843098f,
        -0.227475f, -1.216630f, 1.133818f,
         0.232407f ,  1.119982f, 1.133819f,
         0.232406f ,  1.119982f, 1.602814f,
        -0.227475f,  1.119982f, 1.602813f,
        -0.227475f,  1.119982f, 1.133818f,
         0.232406f , -0.340316f, 2.843098f,
        -0.227475f, -0.340316f, 2.843098f,
         0.232407f , -0.305193f, 1.133819f,
         0.232407f , -0.294496f, 2.297937f,
        -0.227475f, -0.305193f, 1.133818f,
        -0.227475f, -0.294496f, 2.297937f,
         0.232406f , -1.222569f, 1.497195f,
         0.232406f , -0.745504f, 1.477731f,
        -0.227475f, -0.745504f, 1.477731f,
        -0.227475f, -1.222569f, 1.497194f,
        -0.227403f, -0.731178f, 0.901477f,
        -0.227403f, -0.731178f, -0.037620f,
         0.223704f , -0.731178f, -0.037620f,
         0.223704f , -0.731178f, 0.901477f,
        -0.227403f,  1.119377f, 0.901477f,
        -0.227403f,  1.119377f, -0.037620f,
         0.223704f ,  1.119377f, -0.037620f,
         0.223704f ,  1.119377f, 0.901477f,
        -0.227403f, -0.129772f, 0.901477f,
        -0.227403f,  0.551492f, 0.384487f,
        -0.227403f,  1.104268f, 0.408501f,
        -0.227403f,  0.507336f, 0.901477f,
         0.223704f ,  0.507336f, 0.901477f,
         0.223704f ,  1.104269f, 0.408501f,
         0.223704f ,  0.551492f, 0.384487f,
         0.223704f , -0.129772f, 0.901477f,
        -0.227403f,  0.634134f, -0.037620f,
        -0.227403f, -0.066768f, 0.398575f,
        -0.227403f, -0.684649f, 0.389681f,
        -0.227403f, -0.075523f, -0.037620f,
         0.223704f ,  0.634134f, -0.037620f,
         0.223704f , -0.066768f, 0.398575f,
         0.223704f , -0.684649f, 0.389681f,
         0.223704f , -0.075523f, -0.037620f
    };


    Mesh mesh1(vertices, 5, GL_STATIC_DRAW);

    GameObject cube(mesh1);
    Camera      camera;

    while (!glfwWindowShouldClose(window.get()))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window.get(), &cube);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.use();
        

        {
            // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
            shader.setMat4("model", cube.transform.getModel());
            shader.setMat4("view", camera.getView());
            shader.setMat4("projection", camera.getProjection());
            
        }
        cube.draw(shader);
       
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    return 0;
}

