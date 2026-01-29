#include "scop.hpp"

// void processInput(GLFWwindow *window, Camera *camera)
// {
//     float rotationSpeed = 1.5f;
//     float scaleSpeed = 1.0f * deltaTime;

//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//             glfwSetWindowShouldClose(window, true);    

//     //z axis forward
//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//     {
//         camera->setCameraPos({0.0f, 0.0f, -0.1f});
//     }

//     //z axis backward
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//     {
//         camera->setCameraPos({0.0f, 0.0f, 0.1f});
//     }  

//     //x axis left
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//     {
//         camera->setCameraPos({-0.1f, 0.0f, 0.0f});
//     } 

//     //x axis right
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//     {
//         camera->setCameraPos({0.1f, 0.0f, 0.0f});
//     } 
    
// }

int main(int arc, char *argv[])
{
    Window window;
    Camera camera;
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window.get(), &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    Objprop objProp;
    if (!parseObj("resources/teapot2.obj", objProp))
        std::exit(-1);

    Mesh mesh1(objProp, 3, GL_STATIC_DRAW);
    GameObject gameObj(mesh1);
    
    CameraControl &cameraControler = CameraControl::getInstance();
    cameraControler.init(window.get(), &camera);
    glfwSetWindowUserPointer(window.get(), &cameraControler);

    shader.use();
    shader.setVec3("lightDir", glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
    shader.setVec3("lightColor", glm::vec3(1.0f)); //white light

    cameraControler.movementHandler(deltaTime);
    while (!glfwWindowShouldClose(window.get()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;   
          
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        {
            // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
            shader.setMat4("model", gameObj.transform.getModel());
            shader.setMat4("view", camera.getView());
            shader.setMat4("projection", camera.getProjection());

            shader.setVec3("viewPos", camera.getPosition());
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

