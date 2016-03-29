#include "baseLight.h"
#include "shader.h"

using namespace std;

BaseLight::BaseLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                     const glm::vec3 &specular)
  : ambient(ambient)
  , diffuse(diffuse)
  , specular(specular) {}

void BaseLight::init(Shader &shader, const string &name) const {
  shader.bind();
  shader.update((name + ".base.ambient").c_str(), ambient);
  shader.update((name + ".base.diffuse").c_str(), diffuse);
  shader.update((name + ".base.specular").c_str(), specular);
}

string BaseLight::to_string() const {
  string s = "";
  s += "Ambient: (" + std::to_string(ambient.x) + ","
    + std::to_string(ambient.y) + "," + std::to_string(ambient.z) + ")\n";
  s += "Diffuse: (" + std::to_string(diffuse.x) + ","
    + std::to_string(diffuse.y) + "," + std::to_string(diffuse.z) + ")\n";
  s += "Specular: (" + std::to_string(specular.x) + ","
    + std::to_string(specular.y) + "," + std::to_string(specular.z) + ")\n";

  return s;
}
