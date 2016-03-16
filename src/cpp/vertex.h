#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <vector>

class Vertex {
public:
  Vertex(const glm::vec3 &position, const glm::vec2 &texCoord);
  void positions(std::vector<glm::vec3> &positions) const;
  void textures(std::vector<glm::vec2> &texCoords) const;

private:
  glm::vec2 texCoord;
  glm::vec3 position;
  glm::vec3 normal;
};

#endif /* VERTEX_H */
