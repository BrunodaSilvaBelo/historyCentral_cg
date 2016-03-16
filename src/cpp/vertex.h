#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <vector>

class Vertex {
public:
  Vertex(const glm::vec3 &position, const glm::vec2 &texCoord,
         const glm::vec3 &normal = glm::vec3(0.f, 0.f, 0.f));
  void positions(std::vector<glm::vec3> &positions) const;
  void textures(std::vector<glm::vec2> &texCoords) const;
  void normals(std::vector<glm::vec3> &normals) const;

private:
  glm::vec2 texCoord;
  glm::vec3 position;
  glm::vec3 normal;
};

#endif /* VERTEX_H */
