/**
   @mainpage
   @brief
   @author Bruno da Silva Belo
   @bug Luz s� funciona se existir uma directional light
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

    Shader shader("../src/glsl/basicShader");
    Camera camera(glm::vec3(0.f, 0.f, 3.f), 70.f, Window::aspect(), 0.1f, 100.f);

    Transform transform;
    shader.bind();
    shader.update(Shader::PROJECTION, camera.getProjection());

    DirectionalLight dLight({0.f,0.f, 1.f});
    dLight.init(shader);

    vector<glm::vec3> lightPositions = {
      {0.f,0.f,-2.f},
      {3.f, 0.f,-2.f},
      {-3.f,0.f,-2.f},
      {0.f,0.f,-13.f}
    };

    vector<PointLight> pLights;
    for (unsigned i = 0; i < lightPositions.size(); ++i) {
      pLights.emplace_back(lightPositions[i]);
      pLights[i].init(shader);
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

    Window::startTimer();
    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();

      camera.update(Window::deltaTime(), Window::getKey(), Window::getMousePosition()
                    , Window::getMouseButton());
      shader.bind();
      for (auto it : pLights)
        it.bind(shader);

      shader.update(Shader::VIEW, camera.getView());

      for (unsigned i = 0; i < models.size(); ++i) {
        transform.applyTranslate(positions[i]);
        transform.applyRotation({0.f,0.f,0.f});
        transform.applyScale(scales[i]);
        shader.update(Shader::MODEL, transform.getModel());
        models[i].draw(shader);
      }

      transform.applyTranslate(churchPosition + glm::vec3(-0.27f,-2.34f, 1.1f));
      transform.applyScale(glm::vec3(1.f));
      automaticDoor(transform, camera);
      shader.update(Shader::MODEL, transform.getModel());
      door.draw(shader);

      Window::update();
      counter += 0.01f;
    }
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s\n", e.what());
  }
  Window::close();

  return 0;
}
