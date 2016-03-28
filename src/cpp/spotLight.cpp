#include "spotLight.h"
#include <string>
#include "shader.h"

using namespace std;

SpotLight::SpotLight(const glm::vec3 &position, const glm::vec3 &direction,
                     GLboolean isOn, const glm::vec3 &ambient,
                     const glm::vec3 &diffuse, const glm::vec3 &specular,
                     GLfloat constant, GLfloat linear, GLfloat quadratic,
                     GLfloat cutOff, GLfloat outerCutOff)
  : BaseLight(ambient, diffuse, specular, string("spotLight"))
  , isOn(isOn)
  , cutOff(cutOff)
  , outerCutOff(outerCutOff)
  , constant(constant)
  , linear(linear)
  , quadratic(quadratic)
  , position(position)
  , direction(direction) {}

void SpotLight::init(Shader &shader) const {
  string name = "spotlight";
  BaseLight::init(shader);
  shader.update((name + ".constant").c_str(), constant);
  shader.update((name + ".linear").c_str(), linear);
  shader.update((name + ".quadratic").c_str(), linear);
  shader.update((name + ".cutOff").c_str(), cutOff);
  shader.update((name + ".outerCutOff").c_str(), outerCutOff);
}

void SpotLight::bind(Shader &shader) const {
  string name = "spotLight";
  shader.bind();
  shader.update((name + ".position").c_str(), position);
  shader.update((name + ".direction").c_str(), direction);
  shader.update((name + ".isOn").c_str(), isOn);
}

void SpotLight::update(const glm::vec3 &vec, Type type) {
  if (type == DIRECTION)
    direction = vec;
  else if (type == POSITION)
    position = vec;
}

void SpotLight::switchLight() {
  isOn = !isOn;
}
