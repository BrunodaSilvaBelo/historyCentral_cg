#include "mesh.h"
#include "vertex.h"
#include <vector>
#include "obj_loader.h"
#include <stdio.h>

using namespace std;

Mesh::Mesh(pair<Vertex*, GLsizei> vertices, pair<unsigned*, GLsizei> indices) {
  vector<glm::vec3> positions;
  positions.reserve(static_cast<unsigned>(vertices.second));

  vector<glm::vec2> texCoords;
  texCoords.reserve(static_cast<unsigned>(vertices.second));

  vector<glm::vec3> normals;
  normals.reserve(static_cast<unsigned>(vertices.second));

  IndexedModel model;

  for (GLsizei i = 0; i < vertices.second; ++i) {
    vertices.first[i].positions(positions);
    vertices.first[i].textures(texCoords);
    vertices.first[i].normals(normals);
  }

  model.positions = positions;
  model.texCoords = texCoords;
  model.normals = normals;

  for (GLsizei i = 0; i < indices.second; ++i)
    model.indices.push_back(indices.first[i]);

  initMesh(model);
}

Mesh::Mesh(const string &file) {
  IndexedModel model = OBJModel(file).ToIndexedModel();
  initMesh(model);
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::draw() {
  glBindVertexArray(vertexArrayObject);
  glDrawElements(GL_TRIANGLES, counter, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::initMesh(const IndexedModel &model) {
  counter = static_cast<GLsizei>(model.indices.size());

  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);

  glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);

  glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
  glBufferData(GL_ARRAY_BUFFER, model.positions.size()
               * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
  glBufferData(GL_ARRAY_BUFFER, model.positions.size()
               * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
  glBufferData(GL_ARRAY_BUFFER, model.normals.size()
               * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size()
               * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);
  glBindVertexArray(0);
}
