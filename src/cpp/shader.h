#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

class Shader {
public:
  Shader(const std::string &file);
  ~Shader();
  void bind();
private:
  static const unsigned int NUM_SHADERS = 2;
  GLuint program;
  GLuint shaders[NUM_SHADERS];
};


#endif /* SHADER_H */
