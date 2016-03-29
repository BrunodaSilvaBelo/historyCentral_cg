#ifndef ATTENUATION_H
#define ATTENUATION_H

#include <GL/glew.h>
#include <string>

class Shader;

class Attenuation {
public:
  Attenuation(GLfloat constant = 1.f, GLfloat linear = 0.09f,
              GLfloat quadratic = 0.032f);
  void init(Shader &shader, const std::string &name) const;
  std::string to_string() const;
private:
  GLfloat constant;
  GLfloat linear;
  GLfloat quadratic;
};

#endif /* ATTENUATION_H */
