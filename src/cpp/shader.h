#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

class Transform;
class Camera;

class Shader {
public:
  Shader(const std::string &file);
  ~Shader();
  void bind();
  void update(const Transform &transform, const Camera &camera);
private:
  enum {
    TRANSFORM_U,

    NUM_UNIFORMS
  };
  static const unsigned int NUM_SHADERS = 2;
  GLuint program;
  GLuint shaders[NUM_SHADERS];
  GLint uniforms[NUM_UNIFORMS];
};


#endif /* SHADER_H */
