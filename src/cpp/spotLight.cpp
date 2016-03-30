#include "spotLight.h"
#include <string>
#include "shader.h"

using namespace std;

SpotLight::SpotLight(const glm::vec3 &position, const glm::vec3 &direction,
                     GLboolean isOn, const glm::vec3 &ambient,
                     const glm::vec3 &diffuse, const glm::vec3 &specular,
                     GLfloat constant, GLfloat linear, GLfloat quadratic,
                     GLfloat cutOff, GLfloat outerCutOff)
  : isOn(isOn)
  , cutOff(cutOff)
  , outerCutOff(outerCutOff)
  , direction(direction)
  , base(position, constant, linear, quadratic,
         GL_FALSE, ambient, diffuse, specular) {}

void SpotLight::init(Shader &shader) const {
  string name = "spotLight";
  base.init(shader, name + ".base");
  shader.update((name + ".cutOff").c_str(), cutOff);
  shader.update((name + ".outerCutOff").c_str(), outerCutOff);
}

void SpotLight::bind(Shader &shader) const {
  string name = "spotLight";
  shader.bind();
  base.bind(shader, name + ".base");
  shader.update((name + ".direction").c_str(), direction);
  shader.update((name + ".isOn").c_str(), isOn);
}

void SpotLight::update(const glm::vec3 &vec, Type type) {
  if (type == DIRECTION)
    direction = vec;
  else if (type == POSITION)
    base.update(vec);
}

void SpotLight::switchLight() {
  isOn = !isOn;
}

string SpotLight::to_string() const {
  string s = "";
  s += "Direction: (" + std::to_string(direction.x) + ","
    + std::to_string(direction.y) + "," + std::to_string(direction.z) + ")\n";
  s += base.to_string();
  s += "CutOff: " + std::to_string(cutOff) + "\n";
  s += "OuterCutOff: " + std::to_string(outerCutOff) + "\n";

  return s;
}
