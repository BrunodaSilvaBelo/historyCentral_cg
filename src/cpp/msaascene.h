#ifndef MSAASCENE_H
#define MSAASCENE_H

#include <GL/glew.h>
#include "window.h"

template<class Msaa, class Normal, class Scene>
class MsaaScene {
public:
  MsaaScene() {
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
  }

  void run() {
    glEnable(GL_DEPTH_TEST);
    msaa.bind();
    scene.run();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa.buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, normal.buffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    normal.applyTexture();
    scene.pos();
    glBindVertexArray(0);
  }
private:
  Scene scene;
  Msaa msaa;
  Normal normal;
};

#endif /* MSAASCENE_H */
