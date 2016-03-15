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

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mode);

int main() {
  try {
    Window::init("OpenGL", 800, 600);

    Vertex vertices[] = {
      Vertex(glm::vec3(-0.5f, -0.5f, 0.f)),
      Vertex(glm::vec3(0.f, 0.5f, 0.f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.f))
    };

    Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));

    Shader shader("../src/glsl/basicShader");

    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();

      shader.bind();
      mesh.draw();

      Window::update();
    }
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s", e.what());
  }
  Window::close();

  return 0;
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
