#include "vertex.h"

using namespace std;

Vertex::Vertex(const glm::vec3 &position, const glm::vec2 &texCoord,
               const glm::vec3 &normal)
  : texCoord(texCoord)
  , position(position)
  , normal(normal) {}
