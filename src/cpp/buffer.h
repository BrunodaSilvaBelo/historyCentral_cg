#include <GL/glew.h>
#include <stdexcept>
#include <iostream>

namespace components {
  template<class Render, class Texture>
  class Buffer {
  public:
    GLuint buffer;

    Buffer() {
      glGenFramebuffers(1, &buffer);
      glBindFramebuffer(GL_FRAMEBUFFER, buffer);
      render.init();
      texture.init();

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	throw new std::runtime_error("FrameBuffer!");

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~Buffer() {
      glDeleteFramebuffers(1, &buffer);
    }

    void bind() {
      glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    }

    void applyTexture() {
      texture.bind();
    }

  private:
    Render render;
    Texture texture;
  };
}
