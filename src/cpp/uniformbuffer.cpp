#include "uniformbuffer.h"
#include <glm/gtc/type_ptr.hpp>

UniformBuffer::UniformBuffer() {
  glGenBuffers(1, &program);

  glBindBuffer(GL_UNIFORM_BUFFER, program);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr,
               GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferRange(GL_UNIFORM_BUFFER, 0, program, 0, 2 * sizeof(glm::mat4));
}

void UniformBuffer::setProjection(const glm::mat4 &projection) const {
  glBindBuffer(GL_UNIFORM_BUFFER, program);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(projection));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::updateView(const glm::mat4 &view) const {
  glBindBuffer(GL_UNIFORM_BUFFER, program);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(view));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
