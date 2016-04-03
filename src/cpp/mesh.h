#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <vector>
#include "vertex.h"
#include "texture.h"

class Shader;

class Mesh {
 public:
  Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices,
       const std::vector<Texture> &textures);
  ~Mesh();
  void draw(Shader &shader);

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

 private:
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  void setupMesh();
};

#endif /* MESH_H */
