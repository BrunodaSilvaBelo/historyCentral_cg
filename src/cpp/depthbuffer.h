#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include <GL/glew.h>

class DepthBuffer {
public:
  DepthBuffer();
  void resize();
  GLuint dbo;
  GLuint shadows;
private:
  static const GLuint SHADOW_WIDTH;
  static const GLuint SHADOW_HEIGHT;
};


#endif /* DEPTHBUFFER_H */
