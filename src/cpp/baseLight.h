#ifndef BASELIGHT_H
#define BASELIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Shader;

class BaseLight {
public:
  BaseLight(const glm::vec3 &ambient = glm::vec3(0.1f),
            const glm::vec3 &diffuse = glm::vec3(0.5f),
            const glm::vec3 &specular = glm::vec3(1.f));
  void init(Shader &shader, const std::string &name) const;
  std::string to_string() const;
private:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

#endif /* BASELIGHT_H */
