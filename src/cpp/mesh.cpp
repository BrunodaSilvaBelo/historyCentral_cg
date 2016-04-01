#include "mesh.h"
#include "shader.h"

using namespace std;

Mesh::Mesh(const vector<Vertex> &vertices, const vector<GLuint> &indices,
           const vector<Texture> &textures)
  : vertices(vertices)
  , indices(indices)
  , textures(textures) {
  setupMesh();
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               &indices[0], GL_STATIC_DRAW);

  // Position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

  // TexCoord
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid*)(offsetof(Vertex, texCoord)));

  // Normal
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid*)(offsetof(Vertex, normal)));



  glBindVertexArray(0);
}


void Mesh::draw(Shader &shader) {
  GLuint diffuse = 1;
  GLuint specular = 1;
  for (GLuint i = 0; i < textures.size(); ++i) {
    glActiveTexture(GL_TEXTURE0 + i);

    string name = textures[i].type;
    if (name.compare(Texture::DIFFUSE) == 0)
      name += to_string(diffuse++);
    else if (name.compare(Texture::SPECULAR) == 0)
      name += to_string(specular++);

    shader.update(("material." + name).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  for (GLuint i = 0; i < textures.size(); ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}
