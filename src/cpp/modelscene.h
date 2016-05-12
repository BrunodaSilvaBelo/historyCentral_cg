#ifndef MODELSCENE_H
#define MODELSCENE_H

#include "shader.h"
#include "transform.h"
#include <glm/glm.hpp>
#include <vector>
#include "model.h"
#include "window.h"
#include "camera.h"

void automaticDoor(Transform &transform, const Camera &camera);
void automaticDoor(Transform &transform, const Camera &camera) {
  static float rotate = 0.f;
  float direction = 1.f;

  if (glm::distance(transform.getPosition(), camera.getPosition()) >= 5.f)
    direction = -direction;

  rotate += Window::deltaTime() * direction;
  rotate = (rotate <= glm::radians(0.f)) ? glm::radians(0.f) : rotate;
  rotate = (rotate >= glm::radians(90.f)) ? glm::radians(90.f) : rotate;

  transform.applyRotation({0.f, rotate, 0.f});
}

class ModelScene {
public:
  void run() {
    static Shader shader("../src/glsl/model");
    static Transform transform;
    static glm::vec3 churchPosition = {0.f,0.f,0.f};
    static std::vector<glm::vec3> positions = {
      churchPosition,
      churchPosition + glm::vec3(0.f,-2.34f, -6.3f),
    };
    static std::vector<glm::vec3> scales = {
      glm::vec3(1.f),
      glm::vec3(0.1f),
    };

    static std::vector<Model> models = {
      Model("../res/models/church/church.obj"),
      Model("../res/models/nanosuit/nanosuit.obj")
    };

    static Model door("../res/models/church/door.obj");
    if (first) {
      shader.setUniform("Matrices", 0);
      first = false;
    }

    shader.bind();
    for (unsigned i = 0; i < models.size(); ++i) {
      transform.applyTranslate(positions[i]);
      transform.applyRotation({0.f,0.f,0.f});
      transform.applyScale(scales[i]);
      shader.update(Shader::MODEL, transform.getModel());
      models[i].draw(shader);
    }

    transform.applyTranslate(churchPosition + glm::vec3(-0.27f,-2.34f, 1.1f));
    transform.applyScale(glm::vec3(1.f));
    automaticDoor(transform, *Camera::getCamera());
    shader.update(Shader::MODEL, transform.getModel());
    door.draw(shader);
  }
private:
  bool first = true;
};

#endif /* MODELSCENE_H */
