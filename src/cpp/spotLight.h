#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <GL/glew.h>
#include "baseLight.h"
#include <glm/glm.hpp>

class Shader;

class SpotLight : public BaseLight {
public:
  SpotLight(const glm::vec3 &position, const glm::vec3 &direction,
            GLboolean isOn = GL_TRUE,
            const glm::vec3 &ambient = glm::vec3(0.f),
            const glm::vec3 &diffuse = glm::vec3(1.f),
            const glm::vec3 &specular = glm::vec3(1.f),
            GLfloat constant = 1.f, GLfloat linear = 0.09f,
            GLfloat quadratic = 0.032f,
            GLfloat cutOff = glm::cos(glm::radians(12.5f)),
            GLfloat outerCutOff = glm::cos(glm::radians(15.f)));

  enum Type {
    DIRECTION,
    POSITION
  };
  virtual void init(Shader &shader) const;
  virtual void bind(Shader &shader) const;
  void update(const glm::vec3 &vec, Type  type);
  void switchLight();
private:
  GLboolean isOn;
  GLfloat cutOff;
  GLfloat outerCutOff;
  GLfloat constant;
  GLfloat linear;
  GLfloat quadratic;
  glm::vec3 position;
  glm::vec3 direction;
};

#endif /* SPOTLIGHT_H */
