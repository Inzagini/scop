This project has been created as part of the 42 curriculum by `quannguy`.

# 🖤 Scop - Your Minimalist OBJ File Renderer

Scop is a lightweight 3D rendering project developed as part of the 42 curriculum. The goal of the project is to create a simple and efficient application capable of loading and displaying 3D models stored in the Wavefront .obj format.

The project is written in C++ and uses OpenGL for hardware-accelerated rendering. It includes support for .mtl material files, shader-based rendering, and basic camera controls for interacting with the 3D scene.

![Scop Screenshot](resources/screenshot/window.png)

The main objectives of the project are to understand and implement fundamental concepts related to:

3D model parsing and representation.
OpenGL rendering.
Vertex and fragment shaders.
Camera movement and transformations.
Mesh and material handling.
Coordinate systems and 3D mathematics.
User input and real-time interaction.
Scop provides a minimal environment for loading an OBJ model and viewing it interactively in a 3D window.

## Features ✨

- Load and render `.obj` files.
- Support for `.mtl` material files.
- Basic camera controls for navigating the 3D scene.
- Shader-based rendering pipeline.
- Lightweight and easy to use.

## Project Structure 📂

```
scop/
├── .gitignore
├── CMakeLists.txt
├── lsan.supp
├── build/
├── glad/
│   ├── include/
│   │   ├── glad/
│   │   │    └── glad.h
│   │   └── KHR/
│   │   │    └── khrplatform.h
│   └── src/
│       └── glad.c
├── inc/
│   ├── MathUtils.hpp
│   ├── scop.hpp
│   └── class/
│       ├── Camera.hpp
│       ├── CameraControl.hpp
│       ├── GameObject.hpp
│       ├── Mesh.hpp
│       ├── Shader.hpp
│       ├── Transform.hpp
│       └── Window.hpp
├── obj/
├── resources/
│   ├── 42.obj
│   ├── 42.mtl
│   ├── teapot2.obj
│   └── teapot2.mtl
├── shaders/
│   ├── fragment.glsl
│   └── vertex.glsl
└── src/
    ├── CameraControl.cpp
    ├── GameObject.cpp
    ├── main.cpp
    ├── MathUtils.cpp
    ├── Mesh.cpp
    ├── parse.cpp
    ├── processInput.cpp
    ├── Shader.cpp
    ├── Transform.cpp
    └── Window.cpp
```

## Prerequisites 🛠️

- A C++ compiler that supports C++17 or later.
- OpenGL development libraries.
- CMake.

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/scop.git
   cd scop
   ```

2. Configure and build the project with CMake:

   ```bash
   cmake -S . -B build
   cmake --build build
   ```

3. Run the application:
   ```bash
   ./scop [path to .obj file]
   ```

## Usage 🎮

- Place your `.obj` and `.mtl` files in the `resources/` directory or anywhere else.
- Run the application and provide the path to the `.obj` file as a command-line argument:
  ```bash
  ./build/scop resources/your_model.obj
  ```
- Use the following controls to navigate the 3D scene:
  - `W`, `A`, `S`, `D`: Move the object in space.
  - `F`: Toggle texture rendering.
  - Hold and drag the scroll mouse button: Rotate the camera.
  - Scroll the mouse button: Zoom in and out.
  - `Esc`: Exit the application.

## File Descriptions 📜

- `src/`: Contains the source code for the project.
- `inc/`: Header files for the project.
- `glad/`: OpenGL loader library.
- `shaders/`: Vertex and fragment shaders for rendering.
- `resources/`: Example `.obj` and `.mtl` files.
- `CMake`:CMake configuration used to configure and build the project.
- `build/`: Generated build directory created by CMake. It should not be committed to the repository.
- `obj/`: Additional object/build-related files, if generated or used by the project.
- `lsan.supp`: LeakSanitizer suppression configuration for imported libary.

## AI Usage 🤖
AI tools were used as an auxiliary resource during the development of this project.

AI assistance was used for tasks such as:

- Clarifying OpenGL and C++ concepts when documentation or existing resources were difficult to understand.
- Explaining shader concepts and the rendering pipeline.
- Helping with the migration from a Makefile-based build system to CMake.
- Improving and structuring project documentation, including this README.

## License 📄

This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments 🙌

- [GLM](https://github.com/g-truc/glm) - OpenGL Mathematics library.
- [GLAD](https://github.com/Dav1dde/glad) - OpenGL loader generator.
- [LearnOpenGL](https://learnopengl.com/) - For tutorials and resources on OpenGL.
- 42 - For the curriculum and project requirements.

---

Happy rendering! 🎉
