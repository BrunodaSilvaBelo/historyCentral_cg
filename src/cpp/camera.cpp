#include "camera.h"
#include <glm/gtx/transform.hpp>

Camera::Camera(const glm::vec3 &position, float fov, float aspect, float near,
               float far)
  : position(position) {
  forward = glm::vec3(0.f, 0.f, 1.f);
  up = glm::vec3(0.f, 1.f, 0.f);
  projection = glm::perspective(fov, aspect, near, far);
}

glm::mat4 Camera::getViewProjection() const {
  return projection * glm::lookAt(position, position + forward, up);
}
