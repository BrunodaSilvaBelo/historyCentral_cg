#ifndef TEXTUREDENTITY_H
#define TEXTUREDENTITY_H

#include <GL/glew.h>
#include "texture.h"
#include <glm/glm.hpp>
#include <vector>
#include "rawEntity.h"

class Shader;
class Vertex;
class Camera;
class Mesh;
class Transform;
class Mesh;

class TexturedEntity {
public:
  TexturedEntity(const Transform &transform, const Mesh &mesh,
         const std::string &texture);

  void draw(Shader &shader, const Camera &camera) const;
  void update();
  void applyTranslate(const glm::vec3 &translate);
  void applyRotation(const glm::vec3 &rotation);
  void applyScale(const glm::vec3 &scale);
  glm::vec3 getPosition() const;
  void addTexture(const std::string &texture);
private:
  Texture texture;
  RawEntity rawEntity;
};

#endif /* TEXTUREDENTITY_H */
