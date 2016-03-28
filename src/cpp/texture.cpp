#include "texture.h"
#include "stb_image.h"
#include <cassert>
#include <stdio.h>

using namespace std;

Texture::Texture(const string &file) {
  add(file);
}

Texture::~Texture() {
  for (auto &it : textures)
    glDeleteTextures(1, &it);
}

void Texture::add(const string &file) {
  GLsizei width, height, numComponents;
  unsigned char *data = stbi_load(file.c_str(), &width, &height, &numComponents, 4);
  if (!data)
    fprintf(stderr, "Texture loading failed for texture: %s\n", file.c_str());

  textures.emplace_back();
  glGenTextures(1, &textures[count]);
  glBindTexture(GL_TEXTURE_2D, textures[count]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  ++count;
  stbi_image_free(data);
}

void Texture::bind() const {
  for (unsigned i = 0; i < textures.size(); ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i]);
  }
}
