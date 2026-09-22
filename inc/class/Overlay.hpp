#pragma once

struct GLFWwindow;

// Screen-space HUD: a "?" button toggles the controls legend.
class Overlay {
public:
  Overlay();
  ~Overlay();

  Overlay(const Overlay&) = delete;
  Overlay& operator=(const Overlay&) = delete;

  void handleInput(GLFWwindow* window);
  void draw(int screenW, int screenH) const;

private:
  unsigned int program{};
  unsigned int vao{};
  unsigned int vbo{};
  unsigned int ebo{};
  unsigned int buttonQuadCount{};
  unsigned int legendQuadCount{};
  int uScreenLoc{};

  bool visible{false};
  bool mouseWasDown{false};

  float btnX0{}, btnY0{}, btnX1{}, btnY1{};
};
