#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "class/Mesh.hpp"
#include "class/Camera.hpp"
#include "MathUtils.hpp"

class Shader {

    public:
        Shader(const std::string &vertexShaderSourcePath, const std::string &fragmentShaderSourcePath);
        void use(void);
        unsigned int getID();
        
        void setMaterialProp(ObjProp &objprop);
        void setCamera(Camera &camera);
        void setLight();
        void setModel(glm::mat4 model);
        void setModel(Mat4 model);
        void inputHandler(GLFWwindow *window);

    private:
        void colorToggle();
        void ErrorMessage(unsigned int shader, unsigned int type);
        unsigned int createAndCompileShader(unsigned int type, const char* source);
        void attachAndLinkShaders(unsigned int vertexShader, unsigned int fragmentShader);
        std::string loadShader(const std::string &name);

        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setVec4(const std::string &name, float x, float y, float z, float w) const;
        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        void setMat4(const std::string &name, const Mat4 &mat) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, const Vec3 &value) const;
        void setFloat(const std::string &name, const float n) const;
        void setBool(const std::string &name, const bool n) const;

    private:
        unsigned int ID;
        bool        colorEnabled = true;
        
};