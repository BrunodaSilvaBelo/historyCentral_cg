#include "baseLight.h"
#include "shader.h"

using namespace std;

BaseLight::BaseLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                     const glm::vec3 &specular, const string &name)
  : ambient(ambient)
  , diffuse(diffuse)
  , specular(specular)
  , name(name) {}

void BaseLight::init(Shader &shader) const {
  shader.bind();
  shader.update((name + ".ambient").c_str(), ambient);
  shader.update((name + ".diffuse").c_str(), diffuse);
  shader.update((name + ".specular").c_str(), specular);
}
