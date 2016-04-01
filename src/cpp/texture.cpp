#include "texture.h"

using namespace std;

const string Texture::DIFFUSE = "texture_diffuse";
const string Texture::SPECULAR = "texture_specular";

Texture::Texture(GLuint id, const string &type, const aiString &file)
  : id(id)
  , type(type)
  , file(file) {}
