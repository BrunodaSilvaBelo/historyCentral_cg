#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include "texture.h"

class Shader;

class Skybox {
public:
  Skybox(const std::string &path);
  void draw() const;

  GLuint texture;
private:
  GLuint vao;
  GLuint vbo;
};

#endif /* SKYBOX_H */
