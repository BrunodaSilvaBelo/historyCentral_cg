#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class Vertex {
public:
  Vertex(const glm::vec3 &position);
private:
  glm::vec3 position;
};

#endif /* VERTEX_H */
