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

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mode);

int main() {
  try {
    Window::init("OpenGL", 800, 600);

    Vertex vertices[] = {
      Vertex(glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0.f, 0.f)),
      Vertex(glm::vec3(0.f, 0.5f, 0.f), glm::vec2(0.5f, 1.f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.f), glm::vec2(1.f, 0.f)),
    };

    unsigned indices[] = {0, 1, 2};

    Mesh mesh({vertices, sizeof(vertices) / sizeof(vertices[0])},
              {indices, sizeof(indices) / sizeof(indices[0])});
    Mesh mesh2("../res/models/monkey3.obj");
    Shader shader("../src/glsl/basicShader");
    shader.bind();

    Texture texture("../res/textures/wall.jpg");
    texture.bind(0);

    Transform transform;

    Camera camera(glm::vec3(0.f, 0.f, 3.f), 70.f, 800.f/600.f, 0.01f, 1000.f);

    float counter = 0.f;
    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();
      float sin = sinf(counter);
      float cos = cosf(counter);

      transform.applyRotation(glm::vec3(static_cast<GLfloat>(glfwGetTime()),
                                        static_cast<GLfloat>(glfwGetTime()),
                                        static_cast<GLfloat>(glfwGetTime())));
      transform.applyScale(glm::vec3(0.5f, 0.5f, 0.5f));
      shader.update(transform, camera);
      mesh2.draw();

      Window::update();
      counter += 0.01f;
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
