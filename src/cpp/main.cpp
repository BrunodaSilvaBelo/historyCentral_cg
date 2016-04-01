/**
   @mainpage
   @brief
   @author Bruno da Silva Belo
   @bug Luz só funciona se existir uma directional light
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
#include <glm/gtx/transform.hpp>
using namespace std;

int main() {
  try {
    Window::init("OpenGL", 800, 600);

    Shader shader("../src/glsl/basicShader");
    Camera camera(glm::vec3(0.f, 0.f, 3.f), 70.f, Window::aspect(), 0.1f, 100.f);
    Model nano("../res/models/nanosuit/nanosuit.obj");
    Transform transform;
    //transform.applyScale({10.f,10.f,10.f});
    shader.bind();
    shader.update(Shader::PROJECTION, camera.getProjection());

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

    GLfloat counter = 0.f;
    Window::startTimer();
    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();

      camera.update(Window::deltaTime(), Window::getKey(), Window::getMousePosition()
                    , Window::getMouseButton());
      shader.update(Shader::VIEW, camera.getView());
      shader.update(Shader::MODEL, model);
      nano.draw(shader);

      Window::update();
      counter += 0.01f;
    }
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s\n", e.what());
  }
  Window::close();

  return 0;
}
