#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class UniformBuffer {
public:
  UniformBuffer();
  void setProjection(const glm::mat4 &projection) const;
  void updateView(const glm::mat4 &view) const;
private:
  GLuint program;
};

#endif /* UNIFORMBUFFER_H */
