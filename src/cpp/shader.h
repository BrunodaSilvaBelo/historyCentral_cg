#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

class Transform;
class Camera;

class Shader {
public:
  const static char *MODEL;
  const static char *PROJECTION;
  const static char *VIEW;
  Shader(const std::string &file);
  ~Shader();
  void bind();
  void update(const char *name, const glm::mat4 &matrix);
  void update(const char *name, const glm::vec3 &vector);
private:
  static const unsigned int NUM_SHADERS = 2;
  GLuint program;
  GLuint shaders[NUM_SHADERS];
};


#endif /* SHADER_H */
