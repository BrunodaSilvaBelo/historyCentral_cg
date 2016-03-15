#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

class Vertex;

class Mesh {
public:
  Mesh(Vertex *vertices, GLsizei numVertices);
  ~Mesh();
  void draw();

private:
  enum {
    POSITION_VB,

    NUM_BUFFERS
  };
  GLuint vertexArrayObject;
  GLuint vertexArrayBuffers[NUM_BUFFERS];
  GLsizei counter;
};

#endif /* MESH_H */
