#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <GL/glew.h>
#include "baseLight.h"
#include <glm/glm.hpp>
#include <string>

class Shader;

class PointLight : public BaseLight {
public:
  PointLight(const glm::vec3 &position, const glm::vec3 &ambient = glm::vec3(0.05f),
             const glm::vec3 &diffuse = glm::vec3(0.8f),
             const glm::vec3 &specular = glm::vec3(1.f),
             GLfloat constant = 1.f, GLfloat linear = 0.09f,
             GLfloat quadratic = 0.032f);
  virtual void init(Shader &shader) const;
  virtual void bind(Shader &shader) const;
  void update(const glm::vec3 &position);
private:
  static int counter;
  int index;
  GLfloat constant;
  GLfloat linear;
  GLfloat quadratic;
  glm::vec3 position;
};

#endif /* POINTLIGHT_H */
