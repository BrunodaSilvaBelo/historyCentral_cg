#include "directionalLight.h"
#include "shader.h"

using namespace std;

DirectionalLight::DirectionalLight(const glm::vec3 &direction,
                                   const glm::vec3 &ambient,
                                   const glm::vec3 &diffuse,
                                   const glm::vec3 &specular)
  : direction(direction)
  , base(ambient, diffuse, specular) {
}

void DirectionalLight::init(Shader &shader) const {
  base.init(shader, "directionalLight");
}

void DirectionalLight::bind(Shader &shader) const {
  shader.bind();
  shader.update("directionalLight.direction", direction);
}

void DirectionalLight::update(const glm::vec3 &direction) {
  this->direction = direction;
}

string DirectionalLight::to_string() const {
  string s = "";
  s += "Direction: (" + std::to_string(direction.x) + ","
    + std::to_string(direction.y) + "," + std::to_string(direction.z) + ")\n";

  return s + base.to_string();
}
