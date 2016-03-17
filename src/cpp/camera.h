#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <functional>

class Camera {
public:
  Camera(const glm::vec3 &position, float fov, float aspect, float near, float far);
  glm::mat4 getCamera() const;
  glm::mat4 getPerspective() const;
  void update(GLfloat delta, const std::function<int(int)> &getKey,
              const std::function<void(double*,double*)> &getMousePosition);

private:
  GLfloat speed = 5.f;
  GLfloat sensitivity = 0.05f;
  GLfloat yaw = -90.f;
  GLfloat pitch = 0.f;
  glm::vec3 position;
  glm::vec3 forward;
  glm::vec3 up;
  glm::mat4 perspective;
};


#endif /* CAMERA_H */
