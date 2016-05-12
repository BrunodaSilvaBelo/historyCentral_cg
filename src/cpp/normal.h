#ifndef NORMAL_H
#define NORMAL_H

#include <GL/glew.h>

namespace components {
  template<class Derived>
  class BaseTexture {
  public:
    void init() {
      static_cast<Derived*>(this)->init();
    }
    void bind() {
      glBindTexture(GL_TEXTURE_2D, texture);
    }
  protected:
    GLuint texture;
  };

  class Normal : public BaseTexture<Normal> {
  public:
    void init();
  };

  class MsaaTexture : public BaseTexture<MsaaTexture> {
  public:
    void init();
  };
}
#endif /* NORMAL_H */
