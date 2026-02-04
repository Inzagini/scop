#include "scop.hpp"

int main(int arc, char *argv[])
{
    if (arc != 2)
    {
        std::cerr << "NUMBER OF ARGUMENT MUST BE ONLY 1\n";
        std::exit(1);
    }

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
    if (!parseObj(argv[1], objProp))
    {
        std::cerr << "OBJECT PARSING FAILED\n";
        std::exit(-1);
    }

    Mesh mesh1(objProp, 3, GL_STATIC_DRAW);
    GameObject gameObj(mesh1);
    
    CameraControl &cameraControler = CameraControl::getInstance();
    cameraControler.init(window.get(), &camera);
    glfwSetWindowUserPointer(window.get(), &cameraControler);  
    

    while (!glfwWindowShouldClose(window.get()))
    {
        generalInputProcessing(window.get());

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        cameraControler.movementHandler(deltaTime);
        gameObj.inputHandler(window.get(), deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setLight();
        shader.inputHandler(window.get());
        {

            shader.setModel(gameObj.getTransform().getModel());
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

