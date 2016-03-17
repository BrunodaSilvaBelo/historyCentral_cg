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

void Camera::update(GLfloat delta, const std::function<int(int)> &getKey) {
  if (getKey(GLFW_KEY_W) == GLFW_PRESS)
    position += delta * speed * forward;
  if (getKey(GLFW_KEY_S) == GLFW_PRESS)
    position -= delta * speed * forward;
  if (getKey(GLFW_KEY_A) == GLFW_PRESS)
    position -= delta * speed * glm::normalize(glm::cross(forward, up));
  if (getKey(GLFW_KEY_D) == GLFW_PRESS)
    position += delta * speed * glm::normalize(glm::cross(forward, up));
}
