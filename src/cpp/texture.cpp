#include "texture.h"
#include "stb_image.h"
#include <cassert>
#include <stdio.h>

using namespace std;

Texture::Texture(const string &file) {
  GLsizei width, height, numComponents;
  unsigned char *data = stbi_load(file.c_str(), &width, &height, &numComponents, 4);
  if (!data)
    fprintf(stderr, "Texture loading failed for texture: %s\n", file.c_str());

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}

Texture::~Texture() {
  glDeleteTextures(1, &texture);
}

void Texture::bind(unsigned unit) {
  assert(unit <= 31);
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, texture);
}
