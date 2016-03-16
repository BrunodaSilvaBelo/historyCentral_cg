#include "camera.h"
#include <glm/gtx/transform.hpp>

Camera::Camera(const glm::vec3 &pos, float fov, float aspect, float near,
               float far)
  : position(position)
  , forward(glm::vec3(0.f, 0.f, 1.f))
  , up(glm::vec3(0.f, 1.f, 0.f))
  , perspective(glm::perspective(fov, aspect, near, far)) {}

glm::mat4 Camera::getViewProjection() const {
  return perspective * glm::lookAt(position, position + forward, up);
}
