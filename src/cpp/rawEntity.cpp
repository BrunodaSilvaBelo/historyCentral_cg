#include "rawEntity.h"
#include "shader.h"
#include "camera.h"

using namespace std;

RawEntity::RawEntity(const Transform &transform, const Mesh &mesh)
  : transform(transform)
  , mesh(mesh) {}

void RawEntity::draw(Shader &shader, const Camera &camera) const {
  shader.bind();
  shader.update(Shader::MODEL, transform.getModel());
  shader.update(Shader::VIEW, camera.getView());
  mesh.draw();
}

void RawEntity::applyTranslate(const glm::vec3 &translate) {
  transform.applyTranslate(translate);
}

void RawEntity::applyRotation(const glm::vec3 &rotation) {
  transform.applyRotation(rotation);
}

void RawEntity::applyScale(const glm::vec3 &scale) {
  transform.applyScale(scale);
}

glm::vec3 RawEntity::getPosition() const {
  return transform.getPosition();
}
