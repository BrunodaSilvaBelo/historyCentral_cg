#ifndef RAWENTITY_H
#define RAWENTITY_H

#include <glm/glm.hpp>
#include "transform.h"
#include "mesh.h"

class Shader;
class Camera;

class RawEntity {
public:
  RawEntity(const Transform &transform, const Mesh &mesh);

  void draw(Shader &shader, const Camera &camera) const;
  void update();
  void applyTranslate(const glm::vec3 &translate);
  void applyRotation(const glm::vec3 &rotation);
  void applyScale(const glm::vec3 &scale);
  glm::vec3 getPosition() const;

private:
  Transform transform;
  Mesh mesh;
};

#endif /* RAWENTITY_H */
