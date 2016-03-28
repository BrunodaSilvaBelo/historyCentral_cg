#include "mesh.h"
#include "vertex.h"
#include <vector>
#include "obj_loader.h"
#include <stdio.h>
#include <stb_image.h>

using namespace std;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned> indices, bool hasNormal) {

  if (!hasNormal) {
    for (unsigned i = 0; i < indices.size(); i += 3) {
      auto i0 = indices[i];
      auto i1 = indices[i+1];
      auto i2 = indices[i+2];

      auto v1 = vertices[i1] - vertices[i0];
      auto v2 = vertices[i2] - vertices[i0];
      auto normal = glm::cross(v1, v2);

      vertices[i0].addNormal(normal);
      vertices[i1].addNormal(normal);
      vertices[i2].addNormal(normal);
    }
  }

  vector<glm::vec3> positions;
  positions.reserve(static_cast<unsigned>(vertices.size()));

  vector<glm::vec2> texCoords;
  texCoords.reserve(static_cast<unsigned>(vertices.size()));

  vector<glm::vec3> normals;
  normals.reserve(static_cast<unsigned>(vertices.size()));

  IndexedModel model;

  for (auto &i : vertices) {
    i.positions(positions);
    i.textures(texCoords);
    i.normals(normals);
  }

  model.positions = positions;
  model.texCoords = texCoords;
  model.normals = normals;

  for (auto &i : indices)
    model.indices.push_back(i);

  initMesh(model);
}

Mesh::Mesh(const string &file) {
  IndexedModel model = OBJModel(file).ToIndexedModel();
  initMesh(model);
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::draw() const {
  glBindVertexArray(vertexArrayObject);
  glDrawElements(GL_TRIANGLES, counter, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

Mesh Mesh::getPlainTerrain(unsigned size, float width) {
  vector<Vertex> vertices;
  vector<unsigned> indices;
  for (unsigned z = 0; z < size; ++z)
    for (unsigned x = 0; x < size; ++x)
      vertices.push_back(Vertex(glm::vec3(x*width, 0, z*width),
                                glm::vec2(x%2, z%2)));

  for (unsigned z = 0; z < size - 1; ++z)
    for (unsigned x = 0; x < size - 1; ++x) {
      indices.push_back(z  * size + x);
      indices.push_back((z + 1)  * size + x);
      indices.push_back((z + 1) * size + (x + 1));
      indices.push_back((z + 1) * size + (x + 1));
      indices.push_back(z * size + (x + 1));
      indices.push_back(z  * size + x);
    }
  
  return Mesh(vertices, indices);
}

Mesh Mesh::getHeightTerrain(float width, const string &file,
                            float maxHeight) {
  GLsizei filewidth, fileheight, numComponents;
  unsigned char *grey = stbi_load(file.c_str(), &filewidth, &fileheight, &numComponents, 1);
  if (!grey)
    printf("Error on read height map: %s\n", file.c_str());


  for (unsigned i = 0; i < filewidth * fileheight; ++i)
    printf("%d\n", grey[i]);
  printf("%d %d\n", filewidth, fileheight);
  vector<Vertex> vertices;
  vector<unsigned> indices;
  for (unsigned z = 0; z < fileheight; ++z)
    for (unsigned x = 0; x < filewidth; ++x)
      vertices.push_back(Vertex(glm::vec3(x*width,
                                          (static_cast<float>(grey[z*fileheight+x])/255.f)
                                          * maxHeight,
                                          z*width),
                                glm::vec2(x%2, z%2)));

  for (unsigned z = 0; z < fileheight - 1; ++z)
    for (unsigned x = 0; x < filewidth - 1; ++x) {
      indices.push_back(z  * fileheight + x);
      indices.push_back((z + 1)  * fileheight + x);
      indices.push_back((z + 1) * fileheight + (x + 1));
      indices.push_back((z + 1) * fileheight + (x + 1));
      indices.push_back(z * fileheight + (x + 1));
      indices.push_back(z  * fileheight + x);
    }
  stbi_image_free(grey);
  
  return Mesh(vertices, indices);
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
