#include "pointLight.h"
#include <stdexcept>
#include "shader.h"

using namespace std;

int PointLight::counter = 0;

PointLight::PointLight(const glm::vec3 &position, const glm::vec3 &ambient,
                       const glm::vec3 &diffuse, const glm::vec3 &specular,
                       GLfloat constant, GLfloat linear, GLfloat quadratic,
                       GLboolean isPoint)
  : index(counter)
  , position(position)
  , attenuation(constant, linear, quadratic)
  , base(ambient, diffuse, specular) {
  if (isPoint)
    index = counter++;
}

void PointLight::init(Shader &shader, string name) const {
  if (name.compare("") == 0)
    name = string("pointLight[" + std::to_string(index) + "]");

    attenuation.init(shader, name);
  base.init(shader, name);
}

void PointLight::bind(Shader &shader, string name) const {
  shader.bind();
  if (name.compare("") == 0) {
    name = string("pointLight[" + std::to_string(index) + "]");
    shader.update("numPointLight", counter);
  }

  shader.update((name + ".position").c_str(), position);
}

void PointLight::update(const glm::vec3 &position) {
  this->position = position;
}

string PointLight::to_string() const {
  string s = "";
  s += "Position: (" + std::to_string(position.x) + ","
    + std::to_string(position.y) + "," + std::to_string(position.z) + ")\n";

  return s + base.to_string() + attenuation.to_string();
}
