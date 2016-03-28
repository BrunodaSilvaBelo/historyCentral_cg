#include "directionalLight.h"
#include "shader.h"

using namespace std;

DirectionalLight::DirectionalLight(const glm::vec3 &direction,
                                   const glm::vec3 &ambient,
                                   const glm::vec3 &diffuse,
                                   const glm::vec3 &specular)
  : BaseLight(ambient, diffuse, specular, string("directionalLight"))
  , direction(direction) {
}

void DirectionalLight::bind(Shader &shader) const {
  shader.bind();
  shader.update("directionalLight.direction", direction);
}

void DirectionalLight::update(const glm::vec3 &direction) {
  this->direction = direction;
}
