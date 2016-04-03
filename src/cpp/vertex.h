#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
public:
  Vertex() = default;
  Vertex(const glm::vec3 &position,
         const glm::vec2 &texCoord = {0.f,0.f},
         const glm::vec3 &normal = {0.f,0.f,0.f});
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

#endif /* VERTEX_H */
