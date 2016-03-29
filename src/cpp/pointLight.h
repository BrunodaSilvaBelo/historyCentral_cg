#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <GL/glew.h>
#include "baseLight.h"
#include <glm/glm.hpp>
#include <string>
#include "attenuation.h"

class Shader;

class PointLight {
public:
  PointLight(const glm::vec3 &position, const glm::vec3 &ambient = glm::vec3(0.05f),
             const glm::vec3 &diffuse = glm::vec3(0.8f),
             const glm::vec3 &specular = glm::vec3(1.f),
             GLfloat constant = 1.f, GLfloat linear = 0.09f,
             GLfloat quadratic = 0.032f, GLboolean isPoint = GL_TRUE);
  virtual void init(Shader &shader, std::string name = "") const;
  virtual void bind(Shader &shader, std::string name = "") const;
  void update(const glm::vec3 &position);
  std::string to_string() const;
private:
  static int counter;
  int index;
  glm::vec3 position;
  Attenuation attenuation;
  BaseLight base;
};

#endif /* POINTLIGHT_H */
