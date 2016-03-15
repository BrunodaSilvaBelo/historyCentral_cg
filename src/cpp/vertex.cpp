#include "vertex.h"

using namespace std;

Vertex::Vertex(const glm::vec3 &position, const glm::vec2 &texCoord)
  : texCoord(texCoord)
  , position(position) {}

void Vertex::positions(vector<glm::vec3> &positions) const {
  positions.push_back(position);
}

void Vertex::textures(vector<glm::vec2> &texCoords) const {
  texCoords.push_back(texCoord);
}
