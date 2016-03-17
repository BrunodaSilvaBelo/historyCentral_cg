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
      Vertex(glm::vec3(-1.f, -1.f, 0.f), glm::vec2(0.f, 0.f)),
      Vertex(glm::vec3(0.f, -1.f, 1.f), glm::vec2(0.f, 1.f)),
      Vertex(glm::vec3(1.f, -1.f, 0.f), glm::vec2(1.f, 0.f)),
      Vertex(glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 1.f)),
    };

    unsigned indices[] = {0, 3, 1,
                          1, 3, 2,
                          2, 3, 0,
                          0, 1, 2};

    Mesh mesh({vertices, sizeof(vertices) / sizeof(vertices[0])},
              {indices, sizeof(indices) / sizeof(indices[0])});
    Mesh mesh2("../res/models/monkey3.obj");
    Shader shader("../src/glsl/basicShader");
    shader.bind();

    Texture texture("../res/textures/wall.jpg");
    texture.bind(0);

    Transform transform;

    Camera camera(glm::vec3(0.f, 0.f, -5.f), 70.f, Window::aspect(), 0.1f, 100.f);

    shader.update(Shader::PERSPECTIVE, camera.getPerspective());

    GLfloat counter = 0.f;
    Window::startTimer();
    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();
      //transform.applyRotation(glm::vec3(static_cast<GLfloat>(glfwGetTime()),
      //                                  static_cast<GLfloat>(glfwGetTime()),
      //                                  static_cast<GLfloat>(glfwGetTime())));

      shader.update(Shader::WORLD, transform.getModel());
      shader.update(Shader::CAMERA, camera.getCamera());
      mesh2.draw();
      camera.update(Window::deltaTime(), Window::getKey(), Window::getMousePosition());
      Window::update();
      counter += 0.01f;
    }
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s\n", e.what());
  }
  Window::close();

  return 0;
}
