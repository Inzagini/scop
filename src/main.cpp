#include "scop.hpp"

// Returns true on Arch-based Linux.
bool isArch() {
  std::ifstream file("/etc/os-release");

  if (!file.is_open())
    return false;

  std::string line;

  while (std::getline(file, line)) {
    if (line.find("ID=arch") != std::string::npos ||
        line.find("ID_LIKE=arch") != std::string::npos)
      return true;
  }

  return false;
}

// Entry point: set up GL, load the OBJ, run the render loop.
int main(int arc, char* argv[]) {
  if (arc != 2) {
    std::cerr << "NUMBER OF ARGUMENT MUST BE ONLY 1\n";
    std::exit(1);
  }

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    std::exit(-1);
  }

  Window window;
  Camera camera;
  Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
  Overlay overlay;

  if (isArch())
  {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window.get(), &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glEnable(GL_DEPTH_TEST);
  }

  ObjProp objProp;
  if (!parseObj(argv[1], objProp)) {
    std::cerr << "OBJECT PARSING FAILED\n";
    std::exit(-1);
  }

  Mesh mesh1(objProp, 3, GL_STATIC_DRAW);
  GameObject gameObj(mesh1);

  float boundRadius = 0.0f;
  for (std::size_t i = 0; i + 2 < objProp.vertices.size(); i += 3) {
    const float len = MathUtils::length(
        Vec3(objProp.vertices[i], objProp.vertices[i + 1],
             objProp.vertices[i + 2]));
    if (len > boundRadius)
      boundRadius = len;
  }

  constexpr float margin = 1.25f;
  const float halfFov = MathUtils::radians(45.0f / 2.0f);
  const float dist = boundRadius / std::tan(halfFov) * margin;

  camera.setRadius(dist);
  camera.move(MathUtils::normalize(camera.getPosition()) * dist);
  camera.setFarPlane(dist * 5.0f);

  const Vec3 lightDir = MathUtils::normalize(camera.getPosition());

  CameraControl& cameraControler = CameraControl::getInstance();
  cameraControler.init(window.get(), &camera);
  glfwSetWindowUserPointer(window.get(), &cameraControler);

  while (!glfwWindowShouldClose(window.get())) {
    generalInputProcessing(window.get());

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    cameraControler.movementHandler();
    gameObj.inputHandler(window.get(), deltaTime);
    overlay.handleInput(window.get());

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setLight(lightDir);
    shader.inputHandler(window.get());
    {
      shader.setBool("hasTexture", mesh1.hasTexture());

      if (mesh1.hasTexture()) {
        shader.setInt("diffuseTex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh1.getTexture());
      }
      shader.setModel(gameObj.getTransform().getModel());
      shader.setCamera(camera);
      shader.setMaterialProp(objProp);
    }

    gameObj.draw();

    if (shader.isWireframeEnabled() && !shader.isColorEnabled()) {
      shader.setBool("wireframe", true);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glEnable(GL_POLYGON_OFFSET_LINE);
      glPolygonOffset(-1.0f, -1.0f);
      gameObj.draw();
      glDisable(GL_POLYGON_OFFSET_LINE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      shader.setBool("wireframe", false);
    }

    overlay.draw(SCR_WIDTH, SCR_HEIGHT);

    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
