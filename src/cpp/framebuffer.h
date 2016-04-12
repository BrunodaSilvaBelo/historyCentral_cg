#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>

class FrameBuffer {
public:
  FrameBuffer();
  ~FrameBuffer();
  void init();
  GLuint fbo;
  GLuint buffer;

private:
  GLuint rbo;
};

#endif /* FRAMEBUFFER_H */
