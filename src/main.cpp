#include "scop.hpp"

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
        object->transform.setScale(scaleSpeed);
    
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        object->transform.setScale(-scaleSpeed);
}

int main(int arc, char *argv[])
{
    Window window;
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window.get(), &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    Objprop objProp;
    if (!parseObj("resources/teapot2.obj", objProp))
        std::exit(-1);

    Mesh mesh1(objProp, 3, GL_STATIC_DRAW);
    GameObject gameObj(mesh1);
    Camera      camera;

    while (!glfwWindowShouldClose(window.get()))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window.get(), &gameObj);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.use();
        

        {
            // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
            shader.setMat4("model", gameObj.transform.getModel());
            shader.setMat4("view", camera.getView());
            shader.setMat4("projection", camera.getProjection());

            glm::vec3 ambient{1.000000, 1.000000, 1.000000};   // Ka
            glm::vec3 diffuse {0.8, 0.8, 0.8};   // Kd
            glm::vec3 specular {0.8, 0.8, 0.8};  // Ks
            float shininess{0.0};     // Ns
            float opacity{1.0};       // d


            shader.setVec3("lightDir", glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
            shader.setVec3("lightColor", glm::vec3(1.0f));

            shader.setVec3("viewPos", camera.getPos());
            shader.setVec3("material.ambient", objProp.material.ambient);
            shader.setVec3("material.diffuse", objProp.material.diffuse);
            shader.setVec3("material.specular", objProp.material.specular);
            shader.setFloat("material.opacity", objProp.material.opacity);
            shader.setFloat("material.shininess", objProp.material.shininess);

        }
        gameObj.draw(shader);
       
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    return 0;
}

