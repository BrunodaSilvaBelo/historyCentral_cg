#include "camera.h"
#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>

Camera::Camera(const glm::vec3 &position, float fov, float aspect, float near,
               float far)
  : position(position) {
  forward = glm::vec3(0.f, 0.f, 1.f);
  up = glm::vec3(0.f, 1.f, 0.f);
  perspective = glm::perspective(fov, aspect, near, far);
}

glm::mat4 Camera::getCamera() const {
  return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::getPerspective() const {
  return perspective;
}

void Camera::update(GLfloat delta, const std::function<int(int)> &getKey,
                    const std::function<void(double*,double*)> &getMousePosition) {
  if (getKey(GLFW_KEY_W) == GLFW_PRESS)
    position += delta * speed * forward;
  if (getKey(GLFW_KEY_S) == GLFW_PRESS)
    position -= delta * speed * forward;
  if (getKey(GLFW_KEY_A) == GLFW_PRESS)
    position -= delta * speed * glm::normalize(glm::cross(forward, up));
  if (getKey(GLFW_KEY_D) == GLFW_PRESS)
    position += delta * speed * glm::normalize(glm::cross(forward, up));

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
