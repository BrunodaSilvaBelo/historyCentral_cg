#include "depthbuffer.h"
#include <stdexcept>

using namespace std;

const GLuint DepthBuffer::SHADOW_WIDTH = 1024, DepthBuffer::SHADOW_HEIGHT = 1024;

DepthBuffer::DepthBuffer() {
  glGenFramebuffers(1, &dbo);

  glGenTextures(1, &shadows);
  glBindTexture(GL_TEXTURE_2D, shadows);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
               0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindFramebuffer(GL_FRAMEBUFFER, dbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         shadows, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw new runtime_error("FrameBuffer!");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthBuffer::resize() {
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
}
