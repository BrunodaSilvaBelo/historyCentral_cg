#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
  Camera(const glm::vec3 &position, float fov, float aspect, float near, float far);
  glm::mat4 getViewProjection() const;

private:
  glm::vec3 position;
  glm::vec3 forward;
  glm::vec3 up;
  glm::mat4 projection;
};


#endif /* CAMERA_H */
