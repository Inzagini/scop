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
    
}

int main(int arc, char *argv[])
{
    Window window;
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

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
            // glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            // glm::mat4 view          = glm::mat4(1.0f);
            // glm::mat4 projection    = glm::mat4(1.0f);

            // view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            // projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
            // retrieve the matrix uniform locations
            // unsigned int viewLoc  = glGetUniformLocation(shader.getID(), "view");
            
            // pass them to the shaders (3 different ways)
            // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cube.transform.getModel()));
            // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
            
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

