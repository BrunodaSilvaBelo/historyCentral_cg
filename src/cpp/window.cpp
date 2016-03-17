#include "window.h"
#include <stdexcept>

using namespace std;

unique_ptr<GLFWwindow, function<void(GLFWwindow*)>> Window::window
= unique_ptr<GLFWwindow, function<void(GLFWwindow*)>>(nullptr, glfwDestroyWindow);
pair<int,int> Window::dimension = {0,0};
GLfloat Window::currentTimer = 0.f;
GLfloat Window::lastTimer = 0.f;
GLfloat Window::deltaTimer = 0.f;

void Window::init(const string &title, int width, int height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3 );
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  dimension = {width, height};
  window.reset(glfwCreateWindow(dimension.first, dimension.second, title.c_str(), nullptr, nullptr));
  if (!window)
    throw new runtime_error("GLFW window failed to be created!");

  glfwMakeContextCurrent(window.get());
  glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, 1);
  glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    throw new runtime_error("GLEW failed to initialize!");

  glViewport(0,0, dimension.first, dimension.second);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

GLboolean Window::isClosed() {
  return glfwWindowShouldClose(window.get());
}

void Window::update() {
  glfwSwapBuffers(window.get());
  currentTimer = static_cast<GLfloat>(glfwGetTime());
  deltaTimer = currentTimer - lastTimer;
  lastTimer = currentTimer;

  if (getKey()(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window.get(), GL_TRUE);
}

void Window::close() {
  glfwTerminate();
}

void Window::clear() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float Window::aspect() {
  return static_cast<float>(dimension.first)/static_cast<float>(dimension.second);
}

function<int(int)> Window::getKey() {
  GLFWwindow *win = window.get();
  return [win](int key){return glfwGetKey(win, key);};
}

void Window::startTimer() {
  currentTimer = static_cast<GLfloat>(glfwGetTime());
}

GLfloat Window::deltaTime() {
  return deltaTimer;
}
