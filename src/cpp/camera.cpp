#include "camera.h"
#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>

Camera::Camera(const glm::vec3 &position, float fov, float aspect, float near,
               float far)
  : position(position) {
  forward = glm::vec3(0.f, 0.f, -1.f);
  up = glm::vec3(0.f, 1.f, 0.f);
  projection = glm::perspective(fov, aspect, near, far);
}

glm::mat4 Camera::getView() const {
  return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::getProjection() const {
  return projection;
}

glm::vec3 Camera::getPosition() const {
  return position;
}

void Camera::update(GLfloat delta, const std::function<int(int)> &getKey,
                    const std::function<void(double*,double*)> &getMousePosition,
                    const std::function<int(int)> &getMouseButton) {
  auto velocity = speed * delta;
  if (getKey(GLFW_KEY_W) == GLFW_PRESS)
    position += velocity * forward;
  if (getKey(GLFW_KEY_S) == GLFW_PRESS)
    position -= velocity * forward;
  if (getKey(GLFW_KEY_A) == GLFW_PRESS)
    position -= velocity * glm::normalize(glm::cross(forward, up));
  if (getKey(GLFW_KEY_D) == GLFW_PRESS)
    position += velocity * glm::normalize(glm::cross(forward, up));
  if (getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    speed = 100.f;
  else
    speed = 5.f;
  GLdouble xpos, ypos;
  getMousePosition(&xpos, &ypos);
  static GLfloat lastX = static_cast<GLfloat>(xpos);
  static GLfloat lastY = static_cast<GLfloat>(ypos);

  GLfloat xoffset = static_cast<GLfloat>(xpos) - lastX;
  GLfloat yoffset = lastY - static_cast<GLfloat>(ypos);

  lastX = static_cast<GLfloat>(xpos);
  lastY = static_cast<GLfloat>(ypos);

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  pitch = (pitch > 89.f) ? 89.f : pitch;
  pitch = (pitch < -89.f) ? -89.f : pitch;

  forward = glm::normalize(glm::vec3(
                                     cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                                     sin(glm::radians(pitch)),
                                     sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}

glm::vec3 Camera::getForward() const {
  return forward;
}
