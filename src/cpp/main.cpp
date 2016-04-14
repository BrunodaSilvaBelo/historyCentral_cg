/**
   @mainpage
   @brief
   @author Bruno da Silva Belo
   @bug Luz só funciona se existir uma directional light
   @bug Directional light parou de funcionar
*/

#include <GL/glew.h>
#include <stdio.h>
#include <exception>
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include "window.h"
#include "shader.h"
#include "transform.h"
#include "camera.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "model.h"
#include "framebuffer.h"
#include "skybox.h"
#include "uniformbuffer.h"

#define MSAA 1

using namespace std;

void automaticDoor(Transform &transform, const Camera &camera);

void automaticDoor(Transform &transform, const Camera &camera) {
  static float rotate = 0.f;
  float direction = 1.f;

  if (glm::distance(transform.getPosition(), camera.getPosition()) >= 5.f)
    direction = -direction;

  rotate += Window::deltaTime() * direction;
  rotate = (rotate <= glm::radians(0.f)) ? glm::radians(0.f) : rotate;
  rotate = (rotate >= glm::radians(90.f)) ? glm::radians(90.f) : rotate;

  transform.applyRotation({0.f, rotate, 0.f});
}

int main() {
  try {
    Window::init("OpenGL", 800, 600);

    UniformBuffer ub;

    Shader modelShader("../src/glsl/model");
    modelShader.setUniform("Matrices", 0);
    Shader screenShader("../src/glsl/postprocessing");
    Shader skyShader("../src/glsl/skybox");
    skyShader.setUniform("Matrices", 0);

    Camera camera(glm::vec3(0.f, 0.f, 3.f), 70.f, Window::aspect(), 0.1f, 100.f);
    ub.setProjection(camera.getProjection());
    Transform transform;

    DirectionalLight dLight({0.f,0.f, 1.f});
    dLight.init(modelShader);

    vector<glm::vec3> lightPositions = {
      {0.f,0.f,-2.f},
      {3.f, 0.f,-2.f},
      {-3.f,0.f,-2.f},
      {0.f,0.f,-13.f}
    };

    vector<PointLight> pLights;
    for (unsigned i = 0; i < lightPositions.size(); ++i) {
      pLights.emplace_back(lightPositions[i]);
      pLights[i].init(modelShader);
    }

    GLfloat counter = 0.f;
    glm::vec3 churchPosition = {0.f,0.f,0.f};
    vector<glm::vec3> positions = {
      churchPosition,
      churchPosition + glm::vec3(0.f,-2.34f, -6.3f),
    };

    vector<glm::vec3> scales = {
      glm::vec3(1.f),
      glm::vec3(0.1f),
    };

    vector<Model> models = {
      Model("../res/models/church/church.obj"),
      Model("../res/models/nanosuit/nanosuit.obj")
    };

    Model door("../res/models/church/door.obj");

    GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screenShader in Normalized Device Coordinates.
      // Positions   // TexCoords
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 0.0f,
      1.0f, -1.0f,  1.0f, 0.0f,

      -1.0f,  1.0f,  0.0f, 1.0f,
      1.0f, -1.0f,  1.0f, 0.0f,
      1.0f,  1.0f,  1.0f, 1.0f
    };

    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
#if MSAA
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    FrameBuffer fb;
#endif
    Skybox sky("../res/textures/skybox/");
    Window::startTimer();
    while (!Window::isClosed()) {
      glfwPollEvents();
      camera.update(Window::deltaTime(), Window::getKey(), Window::getMousePosition()
                    , Window::getMouseButton());

      ub.updateView(camera.getView());

#if MSAA
      glBindFramebuffer(GL_FRAMEBUFFER, fb.getFBO(GL_TRUE));
      glEnable(GL_DEPTH_TEST);
#endif

      Window::clear();
      skyShader.bind();
      skyShader.update("skybox", 0);
      auto view = glm::mat4(glm::mat3(camera.getView()));
      skyShader.update("skyview", view);
      sky.draw();

      modelShader.bind();
      for (auto it : pLights)
        it.bind(modelShader);

      for (unsigned i = 0; i < models.size(); ++i) {
        transform.applyTranslate(positions[i]);
        transform.applyRotation({0.f,0.f,0.f});
        transform.applyScale(scales[i]);
        modelShader.update(Shader::MODEL, transform.getModel());
        models[i].draw(modelShader);
      }

      transform.applyTranslate(churchPosition + glm::vec3(-0.27f,-2.34f, 1.1f));
      transform.applyScale(glm::vec3(1.f));
      automaticDoor(transform, camera);
      modelShader.update(Shader::MODEL, transform.getModel());
      door.draw(modelShader);

#if MSAA
      glBindFramebuffer(GL_READ_FRAMEBUFFER, fb.getFBO(GL_TRUE));
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.getFBO(GL_FALSE));
      glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      screenShader.bind();
      glBindVertexArray(quadVAO);
      glDisable(GL_DEPTH_TEST);
      glBindTexture(GL_TEXTURE_2D, fb.getTexture(GL_FALSE));	// Use the color attachment texture as the texture of the quad plane
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
#endif


      Window::update();
      counter += 0.01f;
    }
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s\n", e.what());
  }
  Window::close();

  return 0;
}
