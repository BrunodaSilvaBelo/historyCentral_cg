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

int main() {
  try {
    Window::init("OpenGL", 800, 600);

    Vertex vertices[] = {
      Vertex(glm::vec3(-1.f, -1.f, 0.5773f), glm::vec2(0.f, 0.f)),
      Vertex(glm::vec3(0.f, -1.f, -1.15475f), glm::vec2(0.5f, 0.f)),
      Vertex(glm::vec3(1.f, -1.f, 0.5773f), glm::vec2(1.f, 0.f)),
      Vertex(glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.5f, 1.f)),
    };

    unsigned indices[] = {0, 3, 1,
                          1, 3, 2,
                          2, 3, 0,
                          0, 1, 2};

    Mesh mesh({vertices, sizeof(vertices) / sizeof(vertices[0])},
              {indices, sizeof(indices) / sizeof(indices[0])});
    Mesh mesh2("../res/models/spider.obj");
    Shader shader("../src/glsl/basicShader");

    Shader lightShader("../src/glsl/lightShader");

    Texture texture("../res/textures/wall.jpg");
    texture.bind(0);

    Transform transform;
    Transform lightTransform(glm::vec3(1.2f, 1.f, 2.f), glm::vec3(), glm::vec3(0.2f));

    Camera camera(glm::vec3(0.f, 0.f, -5.f), 70.f, Window::aspect(), 0.1f, 100.f);

    shader.bind();
    shader.update(Shader::PROJECTION, camera.getProjection());
    shader.update("objectColor", glm::vec3(1.f, 0.5f, 0.31f));
    shader.update("lightColor", glm::vec3(1.f, 1.f, 1.f));
    lightShader.bind();
    lightShader.update(Shader::PROJECTION, camera.getProjection());

    GLfloat counter = 0.f;
    Window::startTimer();
    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();

      shader.bind();
      transform.applyRotation(glm::vec3(counter, 0.f, 0.f));
      shader.update(Shader::MODEL, transform.getModel());
      shader.update(Shader::VIEW, camera.getView());
      shader.update("lightPosition", lightTransform.getPosition());
      shader.update("viewPosition", camera.getPosition());
      mesh2.draw();

      lightShader.bind();
      lightTransform.applyTranslate(glm::vec3(1.2f, 1.f, 2.f) + glm::vec3(sin(counter * 5) * 3,0.f, 0.f));
      lightShader.update(Shader::MODEL, lightTransform.getModel());
      lightShader.update(Shader::VIEW, camera.getView());
      mesh.draw();

      camera.update(Window::deltaTime(), Window::getKey(), Window::getMousePosition()
                    , Window::getMouseButton());
      Window::update();
      counter += 0.01f;
    }
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s\n", e.what());
  }
  Window::close();

  return 0;
}
