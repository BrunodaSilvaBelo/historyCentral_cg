/**
   @mainpage
   @brief
   @author Bruno da Silva Belo
*/

#include <GL/glew.h>
#include <stdio.h>
#include <exception>
#include "window.h"
#include "shader.h"
#include "mesh.h"
#include "vertex.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include <vector>
#include "entity.h"
#include <glm/gtx/string_cast.hpp>
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"

using namespace std;

int main() {
  try {
    Window::init("OpenGL", 800, 600);

    Mesh mesh("../res/models/esfera.obj");
    Shader shader("../src/glsl/basicShader");
    Camera camera(glm::vec3(0.f, 0.f, 3.f), 70.f, Window::aspect(), 0.1f, 100.f);

    PointLight pLight({0.f,0.f,0.f});
    pLight.init(shader);
    Transform transform;
    Entity planet(transform, mesh, "../res/textures/container2.png");
    planet.addTexture("../res/textures/container2_specular.png");

    shader.bind();
    shader.update(Shader::PROJECTION, camera.getProjection());
    shader.update("material.diffuse", 0);
    shader.update("material.specular", 1);
    shader.update("material.shininess", 32.f);

    GLfloat counter = 0.f;
    Window::startTimer();
    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();

      camera.update(Window::deltaTime(), Window::getKey(), Window::getMousePosition()
                    , Window::getMouseButton());

      planet.applyScale(glm::vec3(5.f));
      planet.applyTranslate({0.f,0.f,0.f});
      planet.draw(shader, camera);
      planet.applyScale(glm::vec3(1.f));
      planet.applyTranslate({sin(counter) * 10, 0.f, cos(counter) * 10});
      planet.draw(shader, camera);
      planet.applyTranslate({-sin(counter*1.5f) * 20, 0.f, cos(counter*1.5f) * 20});
      planet.draw(shader, camera);
      planet.applyScale(glm::vec3(0.2f));
      auto moonPosition = planet.getPosition();
      planet.applyTranslate(glm::vec3(-sin(counter*3) * 2, 0.f, cos(counter*3) * 2)
                            + moonPosition);
      planet.draw(shader, camera);
      planet.applyTranslate(glm::vec3(sin(counter*3) * 4, sin(counter*3) * 4,
                                      cos(counter*3) * 4) + moonPosition);
      planet.draw(shader, camera);

      shader.update("viewPosition", camera.getPosition());
      pLight.bind(shader);

      Window::update();
      counter += 0.01f;
    }
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s\n", e.what());
  }
  Window::close();

  return 0;
}
