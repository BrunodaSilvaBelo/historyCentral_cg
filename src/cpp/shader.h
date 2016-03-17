#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

class Transform;
class Camera;

class Shader {
public:
  const static char *WORLD;
  const static char *PERSPECTIVE;
  const static char *CAMERA;
  const static char *LIGHT_DIRECTION;
  Shader(const std::string &file);
  ~Shader();
  void bind();
  void update(const char *transform, const glm::mat4 &matrix);
private:
  static const unsigned int NUM_SHADERS = 2;
  GLuint program;
  GLuint shaders[NUM_SHADERS];
};


#endif /* SHADER_H */
