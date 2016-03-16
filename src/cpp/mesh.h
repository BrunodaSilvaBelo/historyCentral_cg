#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <utility>
#include <string>

class Vertex;
class IndexedModel;

class Mesh {
public:
  Mesh(std::pair<Vertex*, GLsizei> vertices,
       std::pair<unsigned*, GLsizei> indices);
  Mesh(const std::string &file);
  ~Mesh();
  void draw();

private:
  enum {
    POSITION_VB,
    TEXCOORD_VB,
    INDEX_VB,

    NUM_BUFFERS
  };
  GLuint vertexArrayObject;
  GLuint vertexArrayBuffers[NUM_BUFFERS];
  GLsizei counter;

  void initMesh(const IndexedModel &model);
};

#endif /* MESH_H */
