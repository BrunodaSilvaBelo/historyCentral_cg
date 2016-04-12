#include "framebuffer.h"
#include <stdexcept>

using namespace std;

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
GLuint generateMultiSampleTexture(GLuint samples);

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil) {
  GLenum attachment_type;
  if (!depth && !stencil)
    attachment_type = GL_RGBA;
  else if (depth && !stencil)
    attachment_type = GL_DEPTH_COMPONENT;
  else if (!depth && stencil)
    attachment_type = GL_STENCIL_INDEX;

  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  if (!depth && !stencil)
    glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, 800, 600, 0, attachment_type,
                 GL_UNSIGNED_BYTE, nullptr);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0,
                 GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  return textureId;
}

GLuint generateMultiSampleTexture(GLuint samples) {
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureId);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, 800, 600,
                          GL_TRUE);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  return textureId;
}

FrameBuffer::FrameBuffer() {
  glGenFramebuffers(1, &fbos.first);
  glBindFramebuffer(GL_FRAMEBUFFER, fbos.first);
  textureBuffers.first = generateMultiSampleTexture(4);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D_MULTISAMPLE, textureBuffers.first, 0);

  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8,
                                   800, 600);

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenFramebuffers(1, &fbos.second);
  glBindFramebuffer(GL_FRAMEBUFFER, fbos.second);
  textureBuffers.second = generateAttachmentTexture(false, false);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         textureBuffers.second, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw new runtime_error("FrameBuffer!");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
  glDeleteFramebuffers(1, &fbos.first);
  glDeleteFramebuffers(1, &fbos.second);
}

GLuint FrameBuffer::getFBO(GLboolean multisample) {
  return (multisample) ? fbos.first : fbos.second;
}

GLuint FrameBuffer::getTexture(GLboolean multisample) {
  return (multisample) ? textureBuffers.first : textureBuffers.second;
}
