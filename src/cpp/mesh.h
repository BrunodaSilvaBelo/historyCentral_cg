#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <string>
#include <vector>

class Vertex;
class IndexedModel;

class Mesh {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, bool hasNormal = true);
  Mesh(const std::string &file);
  ~Mesh();
  void draw() const;

  static Mesh getPlainTerrain(unsigned size, float width);
  static Mesh getHeightTerrain(float width, const std::string &file,
                               float maxHeight);
private:
  enum {
    POSITION_VB,
    TEXCOORD_VB,
    NORMAL_VB,

    INDEX_VB,

    NUM_BUFFERS
  };
  GLuint vertexArrayObject;
  GLuint vertexArrayBuffers[NUM_BUFFERS];
  GLsizei counter;

  void initMesh(const IndexedModel &model);
};

#endif /* MESH_H */
