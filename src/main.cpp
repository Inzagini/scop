#include "scop.hpp"
#include "class/Window.hpp"
#include "class/Shader.hpp"
#include "class/Mesh.hpp"
#include <vector>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";



void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
}

int main(int arc, char *argv[])
{
    Window window;
    Shader shader(vertexShaderSource, fragmentShaderSource);

    //veritces
    std::vector<float> firstTriangle = {
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
    };
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
    };
    
    Mesh mesh(firstTriangle, GL_STATIC_DRAW);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, firstTriangle.size() * sizeof(float), firstTriangle.data(), GL_STATIC_DRAW); 
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // glBindVertexArray(VAOS[1]);
    // glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // glEnableVertexAttribArray(0);


    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0); 

    while (!glfwWindowShouldClose(window.get()))
    {
        processInput(window.get());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        shader.use();
        mesh.draw();
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glBindVertexArray(VAOS[1]);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
       
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    // glDeleteVertexArrays(2, VAOS);
    // glDeleteBuffers(2, VBOS);
    // glDeleteBuffers(1, &EBO);

    return 0;
}

