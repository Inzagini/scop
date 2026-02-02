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
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        std::exit(-1);
    }

    Window window;
    Camera camera;
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window.get(), &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glEnable(GL_DEPTH_TEST);

    ObjProp objProp;
    if (!parseObj("resources/teapot2.obj", objProp))
    {

        std::exit(-1);
    }

    Mesh mesh1(objProp, 3, GL_STATIC_DRAW);
    GameObject gameObj(mesh1);
    
    CameraControl &cameraControler = CameraControl::getInstance();
    cameraControler.init(window.get(), &camera);
    glfwSetWindowUserPointer(window.get(), &cameraControler);  

    cameraControler.movementHandler(deltaTime);

    while (!glfwWindowShouldClose(window.get()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;   


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setLight();
        
        {
            shader.setMat4("model", gameObj.getTransform().getModel());

            shader.setCamera(camera);
            shader.setMaterialProp(objProp);
        }
        
        gameObj.draw(shader);
       
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

