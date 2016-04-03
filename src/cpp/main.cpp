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

using namespace std;

int main() {
  try {
    Window::init("OpenGL", 800, 600);

    Shader shader("../src/glsl/basicShader");
    Camera camera(glm::vec3(0.f, 0.f, 3.f), 70.f, Window::aspect(), 0.1f, 100.f);
    Model nano("../res/models/nanosuit/nanosuit.obj");
    Transform transform;
    transform.applyScale(glm::vec3(0.2f));
    shader.bind();
    shader.update(Shader::PROJECTION, camera.getProjection());

    DirectionalLight dLight({0.f,0.f, -1.f});
    dLight.init(shader);
    printf("%s\n", dLight.to_string().c_str());
    PointLight pLight({0.f,0.f,-2.f});
    pLight.init(shader);

    GLfloat counter = 0.f;
    Window::startTimer();
    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();

      camera.update(Window::deltaTime(), Window::getKey(), Window::getMousePosition()
                    , Window::getMouseButton());
      shader.bind();
      pLight.bind(shader);
      shader.update(Shader::VIEW, camera.getView());
      shader.update(Shader::MODEL, transform.getModel());
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
