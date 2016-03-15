#include "window.h"
#include <stdexcept>

using namespace std;

unique_ptr<GLFWwindow, function<void(GLFWwindow*)>> Window::window
= unique_ptr<GLFWwindow, function<void(GLFWwindow*)>>(nullptr, glfwDestroyWindow);

void Window::init(const string &title, int width, int height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3 );
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window.reset(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));
  if (!window)
    throw new runtime_error("GLFW window failed to be created!");

  glfwMakeContextCurrent(window.get());

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    throw new runtime_error("GLEW failed to initialize!");

  glViewport(0,0, width, height);
}

GLboolean Window::isClosed() {
  return glfwWindowShouldClose(window.get());
}

void Window::update() {
  glfwSwapBuffers(window.get());
}

void Window::close() {
  glfwTerminate();
}

void Window::clear() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
}
