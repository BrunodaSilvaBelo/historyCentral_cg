#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <vector>

class Texture {
public:
  Texture() = default;
  Texture(const std::string &file);
  ~Texture();

  void add(const std::string &file);
  void bind() const;

private:
  unsigned count = 0;
  std::vector<GLuint> textures;
};


#endif /* TEXTURE_H */
