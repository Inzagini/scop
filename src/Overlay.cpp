#include "class/Overlay.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "stb_easy_font.h"

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

struct HudVertex {
  float x, y, z;
  unsigned char r, g, b, a;
};
static_assert(sizeof(HudVertex) == 16, "must match stb_easy_font layout");

std::string loadFile(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Cannot open shader file: " << path << '\n';
    std::exit(-1);
  }
  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

unsigned compileShader(unsigned type, const std::string& src) {
  unsigned shader = glCreateShader(type);
  const char* c = src.c_str();
  glShaderSource(shader, 1, &c, nullptr);
  glCompileShader(shader);

  int ok{};
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    char log[512];
    glGetShaderInfoLog(shader, 512, nullptr, log);
    std::cerr << "HUD shader compile error:\n" << log << '\n';
    std::exit(-1);
  }
  return shader;
}

unsigned makeProgram(const std::string& vsrc, const std::string& fsrc) {
  unsigned vs = compileShader(GL_VERTEX_SHADER, vsrc);
  unsigned fs = compileShader(GL_FRAGMENT_SHADER, fsrc);

  unsigned program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  int ok{};
  glGetProgramiv(program, GL_LINK_STATUS, &ok);
  if (!ok) {
    char log[512];
    glGetProgramInfoLog(program, 512, nullptr, log);
    std::cerr << "HUD program link error:\n" << log << '\n';
    std::exit(-1);
  }
  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

// Append the glyph quads of `text` (colored white) to `verts`, return #quads.
unsigned appendText(std::vector<HudVertex>& verts, float x, float y,
                    char* text) {
  std::vector<char> buf(100000); // ~270 bytes/char, plenty
  const int quads =
      stb_easy_font_print(x, y, text, nullptr, buf.data(),
                          static_cast<int>(buf.size()));
  const HudVertex* v = reinterpret_cast<const HudVertex*>(buf.data());
  verts.insert(verts.end(), v, v + quads * 4);
  return static_cast<unsigned>(quads);
}

} // namespace

Overlay::Overlay() {
  program = makeProgram(loadFile("shaders/hud.vert"),
                        loadFile("shaders/hud.frag"));
  uScreenLoc = glGetUniformLocation(program, "uScreen");

  const float scale = 3.0f;      // stb font is ~7px tall natively
  const float margin = 4.0f;     // distance from window corner (native units)
  const float buttonSize = 12.0f; // native units
  const float pad = 6.0f;        // text inset inside the legend panel

  std::vector<HudVertex> verts;
  auto pushQuad = [&](float x0, float y0, float x1, float y1, unsigned char r,
                      unsigned char g, unsigned char b, unsigned char a) {
    verts.push_back({x0, y0, 0, r, g, b, a});
    verts.push_back({x1, y0, 0, r, g, b, a});
    verts.push_back({x1, y1, 0, r, g, b, a});
    verts.push_back({x0, y1, 0, r, g, b, a});
  };

  // --- "?" button (always visible) ---
  pushQuad(margin, margin, margin + buttonSize, margin + buttonSize, 40, 40, 60,
           220);
  char buttonLabel[] = "?";
  buttonQuadCount =
      1 + appendText(verts, margin + 3.0f, margin + 1.0f, buttonLabel);

  btnX0 = margin * scale;
  btnY0 = margin * scale;
  btnX1 = (margin + buttonSize) * scale;
  btnY1 = (margin + buttonSize) * scale;

  // --- controls legend (hidden until toggled) ---
  char legend[] =
      "Controls:\n"
      "  W A S D      move object\n"
      "  Q / E        rotate object (Y)\n"
      "  Arrow keys   rotate object (X/Z)\n"
      "  Space        center object\n"
      "  Left-drag    rotate object\n"
      "  Middle-drag  orbit camera\n"
      "  Scroll       zoom\n"
      "  F            toggle color\n"
      "  G            toggle wireframe\n"
      "  Esc          quit";

  const float textW = static_cast<float>(stb_easy_font_width(legend));
  const float textH = static_cast<float>(stb_easy_font_height(legend));
  const float panelX = margin;
  const float panelY = margin + buttonSize + margin;

  pushQuad(panelX, panelY, panelX + textW + 2 * pad, panelY + textH + 2 * pad,
           20, 20, 30, 200);
  legendQuadCount =
      1 + appendText(verts, panelX + pad, panelY + pad, legend);

  // Scale the native font up to a readable size.
  for (auto& v : verts) {
    v.x *= scale;
    v.y *= scale;
  }

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(HudVertex),
               verts.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(HudVertex),
                        (void*)offsetof(HudVertex, x));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(HudVertex),
                        (void*)offsetof(HudVertex, r));
  glEnableVertexAttribArray(1);

  std::vector<unsigned> indices;
  const unsigned totalQuads = buttonQuadCount + legendQuadCount;
  indices.reserve(totalQuads * 6);
  for (unsigned q = 0; q < totalQuads; ++q) {
    const unsigned base = q * 4;
    indices.push_back(base + 0);
    indices.push_back(base + 1);
    indices.push_back(base + 2);
    indices.push_back(base + 2);
    indices.push_back(base + 3);
    indices.push_back(base + 0);
  }

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned),
               indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
}

Overlay::~Overlay() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteProgram(program);
}

void Overlay::handleInput(GLFWwindow* window) {
  const bool down =
      glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

  if (down && !mouseWasDown) {
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    if (mx >= btnX0 && mx <= btnX1 && my >= btnY0 && my <= btnY1)
      visible = !visible;
  }
  mouseWasDown = down;
}

void Overlay::draw(int screenW, int screenH) const {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glUseProgram(program);
  glUniform2f(uScreenLoc, static_cast<float>(screenW),
              static_cast<float>(screenH));

  glBindVertexArray(vao);

  // "?" button is always drawn.
  glDrawElements(GL_TRIANGLES, buttonQuadCount * 6, GL_UNSIGNED_INT, nullptr);

  // Legend only when toggled on.
  if (visible)
    glDrawElements(GL_TRIANGLES, legendQuadCount * 6, GL_UNSIGNED_INT,
                   reinterpret_cast<void*>(buttonQuadCount * 6 *
                                           sizeof(unsigned)));

  glBindVertexArray(0);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}
