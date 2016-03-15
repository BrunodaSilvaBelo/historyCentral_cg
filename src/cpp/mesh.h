#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <utility>

class Vertex;

class Mesh {
public:
  Mesh(std::pair<Vertex*, GLsizei> vertices);
  ~Mesh();
  void draw();

private:
  enum {
    POSITION_VB,
    TEXCOORD_VB,

    NUM_BUFFERS
  };
  GLuint vertexArrayObject;
  GLuint vertexArrayBuffers[NUM_BUFFERS];
  GLsizei counter;
};

#endif /* MESH_H */
