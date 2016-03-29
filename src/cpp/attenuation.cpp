#include "attenuation.h"
#include "shader.h"

using namespace std;

Attenuation::Attenuation(GLfloat constant, GLfloat linear, GLfloat quadratic)
  : constant(constant)
  , linear(linear)
  , quadratic(quadratic) {}

void Attenuation::init(Shader &shader, const string &name) const {
  shader.update((name + ".attenuation.constant").c_str(), constant);
  shader.update((name + ".attenuation.linear").c_str(), linear);
  shader.update((name + ".attenuation.quadratic").c_str(), quadratic);
}

string Attenuation::to_string() const {
  string s = "";
  s += "Constant: " + std::to_string(constant);
  s += "\nLinear: " + std::to_string(linear);
  s += "\nQuadratic: " + std::to_string(quadratic);

  return s + "\n";
}
