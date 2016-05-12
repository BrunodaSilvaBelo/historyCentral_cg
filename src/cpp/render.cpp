#include <GL/glew.h>
#include "render.h"

using components::Render;
using components::MsaaRender;

void Render::init() {
  GLuint render;
  glGenRenderbuffers(1, &render);
  glBindRenderbuffer(GL_RENDERBUFFER, render);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			    GL_RENDERBUFFER, render);
}

void MsaaRender::init() {
  GLuint render;
  glGenRenderbuffers(1, &render);
  glBindRenderbuffer(GL_RENDERBUFFER, render);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8,
				   800, 600);
}
