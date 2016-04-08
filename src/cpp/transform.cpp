#include "transform.h"
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

Transform::Transform(const glm::vec3 &translate, const glm::vec3 &rotation,
                     const glm::vec3 &scale)
  : translate(translate)
  , rotation(rotation)
  , scale(scale) {}

glm::mat4 Transform::getModel() const {
  glm::mat4 tMatrix = glm::translate(translate);

  glm::mat4 rotXMatrix = glm::rotate(rotation.x, glm::vec3(1.f, 0.f, 0.f));
  glm::mat4 rotYMatrix = glm::rotate(rotation.y, glm::vec3(0.f, 1.f, 0.f));
  glm::mat4 rotZMatrix = glm::rotate(rotation.z, glm::vec3(0.f, 0.f, 1.f));

  glm::mat4 sMatrix = glm::scale(scale);

  return tMatrix * rotZMatrix * rotYMatrix * rotXMatrix * sMatrix;
}

void Transform::applyTranslate(const glm::vec3 &translate) {
  this->translate = translate;
}

void Transform::applyRotation(const glm::vec3 &rotation) {
  this->rotation = rotation;
}

void Transform::applyScale(const glm::vec3 &scale) {
  this->scale = scale;
}

glm::vec3 Transform::getPosition() const {
  return translate;
}
