#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <assimp/scene.h>

struct Texture {
public:
  Texture() = default;
  Texture(GLuint id, const std::string &type, const aiString &file);

  static const std::string DIFFUSE;
  static const std::string SPECULAR;
  GLuint id;
  std::string type;
  aiString file;
};


#endif /* TEXTURE_H */
