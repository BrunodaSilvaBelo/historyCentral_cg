#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

class Transform {
public:
  Transform(const glm::vec3 &translate = glm::vec3(),
            const glm::vec3 &rotation = glm::vec3(),
            const glm::vec3 &scale = glm::vec3(1.f, 1.f, 1.f));
  glm::mat4 getModel() const;
  void applyTranslate(const glm::vec3 &translate);
  void applyRotation(const glm::vec3 &rotation);
  void applyScale(const glm::vec3 &scale);
  glm::vec3 getPosition() const;

private:
  glm::vec3 translate;
  glm::vec3 rotation;
  glm::vec3 scale;
};

#endif /* TRANSFORM_H */
