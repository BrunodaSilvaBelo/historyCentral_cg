#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <GL/glew.h>
#include "baseLight.h"
#include <glm/glm.hpp>
#include <string>

class Shader;

class DirectionalLight : public BaseLight {
public:
  DirectionalLight( const glm::vec3 &direction,
                    const glm::vec3 &ambient = glm::vec3(0.05f),
                    const glm::vec3 &diffuse = glm::vec3(0.4f),
                    const glm::vec3 &specular = glm::vec3(0.5f));
  ~DirectionalLight() {}
  virtual void bind(Shader &shader) const;
  void update(const glm::vec3 &direction);
private:
  glm::vec3 direction;
};

#endif /* DIRECTIONALLIGHT_H */
