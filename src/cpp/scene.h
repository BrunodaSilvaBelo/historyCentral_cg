#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"
#include "shader.h"
#include "transform.h"
#include "camera.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "model.h"
#include "framebuffer.h"
#include "uniformbuffer.h"
#include "window.h"
#include "skybox.h"

void automaticDoor(Transform &transform, const Camera &camera);

namespace components {
  class NoScene {
  public:
    void run() {}
  };

  template<class Scene = NoScene>
  class SkyboxScene {
  public:
    void run() {
      static Shader shader("../src/glsl/skybox");
      if (first) {
	shader.setUniform("Matrices", 0);
	first = false;
      }
      glEnable(GL_DEPTH_TEST);
      shader.bind();
      shader.update("skybox", 0);

      static Skybox sky("../res/textures/skybox/");
      sky.draw();

      static Scene scene;
      scene.run();
    }

  private:
    bool first = true;
  };

  template<class Msaa, class Normal, class Infixed>
  class Scene {
  public:
    void run() {
      static UniformBuffer ub;

      static Shader modelShader("../src/glsl/model");

      static Shader screenShader("../src/glsl/postprocessing");

      static Camera camera(glm::vec3(0.f, 0.f, 5.f), 70.f, Window::aspect(), 0.1f, 100.f);

      static Transform transform;

      static DirectionalLight dLight({0.f,0.f, 1.f});


      static std::vector<glm::vec3> lightPositions = {
	{0.f,0.f,-2.f},
	{3.f, 0.f,-2.f},
	{-3.f,0.f,-2.f},
	{0.f,0.f,-13.f}
      };

      static std::vector<PointLight> pLights;

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

      static GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screenShader in Normalized Device Coordinates.
	// Positions   // TexCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
      };

      static GLuint quadVAO, quadVBO;

      static Msaa msaa;
      static Normal normal;

      if (first) {
	modelShader.setUniform("Matrices", 0);
	ub.setProjection(camera.getProjection());
	dLight.init(modelShader);
	for (unsigned i = 0; i < lightPositions.size(); ++i) {
	  pLights.emplace_back(lightPositions[i]);
	  pLights[i].init(modelShader);
	}
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	first = false;
      }

      camera.update(Window::deltaTime(), Window::getKey(), Window::getMousePosition()
                    , Window::getMouseButton());

      ub.updateView(camera.getView());

      glEnable(GL_DEPTH_TEST);
      msaa.bind();

      static Infixed in;
      in.run();

      modelShader.bind();
      dLight.bind(modelShader);
      for (auto it : pLights)
        it.bind(modelShader);

      for (unsigned i = 0; i < models.size(); ++i) {
        transform.applyTranslate(positions[i]);
        transform.applyRotation({0.f,0.f,0.f});
        transform.applyScale(scales[i]);
        modelShader.update(Shader::MODEL, transform.getModel());
        models[i].draw(modelShader);
      }

      transform.applyTranslate(churchPosition + glm::vec3(-0.27f,-2.34f, 1.1f));
      transform.applyScale(glm::vec3(1.f));
      automaticDoor(transform, camera);
      modelShader.update(Shader::MODEL, transform.getModel());
      door.draw(modelShader);

      glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa.buffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, normal.buffer);
      glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      normal.applyTexture();
      screenShader.bind();
      glBindVertexArray(quadVAO);
      glDisable(GL_DEPTH_TEST);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);


    }
  private:
    bool first = true;
  };

}

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
#endif /* SCENE_H */
