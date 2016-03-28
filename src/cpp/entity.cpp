#include "entity.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"

using namespace std;

Entity::Entity(const Transform &transform, const Mesh &mesh, const string &texture)
  : transform(transform)
  , mesh(mesh)
  , texture(texture) {
}

void Entity::draw(Shader &shader, const Camera &camera) const {
  texture.bind();
  shader.bind();
  shader.update(Shader::MODEL, transform.getModel());
  shader.update(Shader::VIEW, camera.getView());
  mesh.draw();
}

void Entity::applyTranslate(const glm::vec3 &translate) {
  transform.applyTranslate(translate);
}

void Entity::applyRotation(const glm::vec3 &rotation) {
  transform.applyRotation(rotation);
}

void Entity::applyScale(const glm::vec3 &scale) {
  transform.applyScale(scale);
}

glm::vec3 Entity::getPosition() const {
  return transform.getPosition();
}

void Entity::addTexture(const string &texture) {
  this->texture.add(texture);
}
