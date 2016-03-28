#ifndef BASELIGHT_H
#define BASELIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Shader;

class BaseLight {
public:
  BaseLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
            const glm::vec3 &specular, const std::string &name);
  virtual ~BaseLight() {}
  virtual void init(Shader &shader) const;
  virtual void bind(Shader &shader) const = 0;

private:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  std::string name;
};

#endif /* BASELIGHT_H */
