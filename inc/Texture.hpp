#define STB_IMAGE_IMPLEMENTATION
#include "glad/glad.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

namespace {

unsigned int loadTexture2D(const std::string& path) {
  int w, h, channels;
  stbi_set_flip_vertically_on_load(true); // OBJ v-origin is bottom-left
  unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
  if (!data) {
    std::cerr << "Failed to load texture: " << path << '\n';
    return 0;
  }

  unsigned int tex = 0;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  const GLenum fmt = (channels == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  return tex;
}

} // namespace
