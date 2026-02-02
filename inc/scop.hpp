#pragma once

# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include <iostream>
#include <vector>
#include <cmath>

#include "class/Window.hpp"
#include "class/Shader.hpp"
#include "class/Mesh.hpp"
#include "class/GameObject.hpp"
#include "class/Camera.hpp"
#include "class/CameraControl.hpp"

float deltaTime = 0.0f;
float lastFrame = 0.0f;


bool parseObj(const char *, ObjProp &obj);

class Window;


