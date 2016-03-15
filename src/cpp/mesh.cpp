#include "mesh.h"
#include "vertex.h"
#include <vector>

using namespace std;

Mesh::Mesh(pair<Vertex*, GLsizei> vertices) {
  counter = vertices.second;

  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);

  vector<glm::vec3> positions;
  positions.reserve(static_cast<unsigned>(vertices.second));

  vector<glm::vec2> texCoords;
  texCoords.reserve(static_cast<unsigned>(vertices.second));

  for (GLsizei i = 0; i < vertices.second; ++i) {
    vertices.first[i].positions(positions);
    vertices.first[i].textures(texCoords);
  }

  glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
  glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
  glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long>(vertices.second)
               * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
  glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long>(vertices.second)
               * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::draw() {
  glBindVertexArray(vertexArrayObject);
  glDrawArrays(GL_TRIANGLES, 0, counter);
  glBindVertexArray(0);
}
