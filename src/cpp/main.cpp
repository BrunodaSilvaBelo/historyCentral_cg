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

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mode);

int main() {
  try {
    Window::init("OpenGL", 800, 600);

    Vertex vertices[] = {
      Vertex(glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0.f, 0.f)),
      Vertex(glm::vec3(0.f, 0.5f, 0.f), glm::vec2(0.5f, 1.f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.f), glm::vec2(1.f, 0.f))
    };


    Mesh mesh({vertices, sizeof(vertices) / sizeof(vertices[0])});
    Shader shader("../src/glsl/basicShader");
    Texture texture("../res/textures/wall.jpg");
    Transform transform;

    float counter = 0.f;
    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();
      transform.applyTranslate(glm::vec3(sinf(counter), 0.f, 0.f));
      shader.bind();
      shader.update(transform);
      texture.bind(0);
      mesh.draw();

      Window::update();
      counter += 0.1f;
    }
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s\n", e.what());
  }
  Window::close();

  return 0;
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
