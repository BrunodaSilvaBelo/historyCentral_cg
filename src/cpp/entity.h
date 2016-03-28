#ifndef ENTITY_H
#define ENTITY_H

#include <GL/glew.h>
#include "transform.h"
#include "mesh.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <vector>

class Shader;
class Vertex;
class Camera;
class Mesh;

class Entity {
public:
  Entity(const Transform &transform, const Mesh &mesh,
         const std::string &texture);

  void draw(Shader &shader, const Camera &camera) const;
  void update();
  void applyTranslate(const glm::vec3 &translate);
  void applyRotation(const glm::vec3 &rotation);
  void applyScale(const glm::vec3 &scale);
  glm::vec3 getPosition() const;
  void addTexture(const std::string &texture);
private:
  Transform transform;
  Mesh mesh;
  Texture texture;
};

#endif /* ENTITY_H */
