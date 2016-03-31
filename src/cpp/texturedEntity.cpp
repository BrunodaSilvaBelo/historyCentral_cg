#include "texturedEntity.h"
#include "shader.h"
#include "camera.h"

using namespace std;

TexturedEntity::TexturedEntity(const Transform &transform, const Mesh &mesh,
                               const string &texture)
  : texture(texture)
  , rawEntity(transform, mesh) {
}

void TexturedEntity::draw(Shader &shader, const Camera &camera) const {
  texture.bind();
  rawEntity.draw(shader, camera);
}

void TexturedEntity::applyTranslate(const glm::vec3 &translate) {
  rawEntity.applyTranslate(translate);
}

void TexturedEntity::applyRotation(const glm::vec3 &rotation) {
  rawEntity.applyRotation(rotation);
}

void TexturedEntity::applyScale(const glm::vec3 &scale) {
  rawEntity.applyScale(scale);
}

glm::vec3 TexturedEntity::getPosition() const {
  return rawEntity.getPosition();
}

void TexturedEntity::addTexture(const string &texture) {
  this->texture.add(texture);
}
