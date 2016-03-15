#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture {
public:
  Texture(const std::string &file);
  ~Texture();

  void bind(unsigned unit);

private:
  GLuint texture;
};


#endif /* TEXTURE_H */
