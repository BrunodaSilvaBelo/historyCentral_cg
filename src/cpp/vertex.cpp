#include "vertex.h"

using namespace std;

Vertex::Vertex(const glm::vec3 &position, const glm::vec2 &texCoord,
               const glm::vec3 &normal)
  : texCoord(texCoord)
  , position(position)
  , normal(normal) {}

void Vertex::positions(vector<glm::vec3> &positions) const {
  positions.push_back(position);
}

void Vertex::textures(vector<glm::vec2> &texCoords) const {
  texCoords.push_back(texCoord);
}

void Vertex::normals(vector<glm::vec3> &normals) const {
  normals.push_back(normal);
}

void Vertex::addNormal(const glm::vec3 &normal) {
  this->normal += normal;
}

glm::vec3 Vertex::operator-(const Vertex &rhs) const {
  return position - rhs.position;
}
