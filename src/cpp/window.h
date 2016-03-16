#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <string>
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include <utility>

class Window {
public:
  static void init(const std::string &title, int width, int height);
  static GLboolean isClosed();
  static void update();
  static void close();
  static void clear();
  static float aspect();
private:
  static constexpr GLint FPS = 1000 / 60;
  static std::pair<int, int> dimension;
  static std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> window;
};

#endif /* WINDOW_H */
