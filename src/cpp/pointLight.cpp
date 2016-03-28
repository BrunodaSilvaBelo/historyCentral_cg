#include "pointLight.h"
#include <stdexcept>
#include "shader.h"

using namespace std;

int PointLight::counter = 0;

PointLight::PointLight(const glm::vec3 &position, const glm::vec3 &ambient,
                       const glm::vec3 &diffuse, const glm::vec3 &specular,
                       GLfloat constant, GLfloat linear, GLfloat quadratic)
: BaseLight(ambient, diffuse, specular, string("pointLight[" + to_string(counter) + "]"))
, index(counter)
, constant(constant)
, linear(linear)
, quadratic(quadratic)
, position(position) {
  index = counter++;
}

void PointLight::init(Shader &shader) const {
  string name = string("pointLight[" + to_string(index) + "]");
  BaseLight::init(shader);
  shader.update((name + ".constant").c_str(), constant);
  shader.update((name + ".linear").c_str(), linear);
  shader.update((name + ".quadratic").c_str(), linear);
}

void PointLight::bind(Shader &shader) const {
  string name = string("pointLight[" + to_string(index) + "]");
  shader.bind();
  shader.update((name + ".position").c_str(), position);
  shader.update("numPointLight", counter);
}

void PointLight::update(const glm::vec3 &position) {
  this->position = position;
}
