#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <utility>

class FrameBuffer {
public:
  FrameBuffer();
  ~FrameBuffer();
  void init();
  GLuint getFBO(GLboolean multisample);
  GLuint getTexture(GLboolean multisample);
private:
  GLuint rbo;
  std::pair<GLuint,GLuint> fbos;
  std::pair<GLuint,GLuint> textureBuffers;
};

#endif /* FRAMEBUFFER_H */
