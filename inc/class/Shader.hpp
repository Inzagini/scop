#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class Shader {

    public:
        Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
        void use(void);


    private:
        void ErrorMessage(unsigned int shader, unsigned int type);
        unsigned int createAndCompileShader(unsigned int type, const char* source);
        void attachAndLinkShaders(unsigned int vertexShader, unsigned int fragmentShader);

    private:
        unsigned int ID;
};