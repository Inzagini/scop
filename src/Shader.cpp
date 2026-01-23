#include "class/Shader.hpp"


Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Failed to init GLAD\n";
        std::exit (-1);
    }

    unsigned int vertexShader = createAndCompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = createAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    attachAndLinkShaders(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(this->ID);
}

unsigned int Shader::createAndCompileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    ErrorMessage(shader, type);
    return shader;
}

void Shader::attachAndLinkShaders(unsigned int vertexShader, unsigned int fragmentShader)
{
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);

    int success;
    char infolog[512];
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);
        std::cerr << "ERROR SHARDER FRAGMENT COMPILATION ERROR\n" << infolog;
    }
}

void Shader::ErrorMessage(unsigned int shader, unsigned int type)
{
    int succeded{};
    char infolog[512];
    std::string m = type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";

    glGetShaderiv(shader, GL_COMPILE_STATUS, &succeded);
    if (!succeded)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infolog);
        std::cerr << "ERROR SHARDER " << m << " COMPILATION ERROR\n" << infolog;
    }
}