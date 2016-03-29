/**
   @mainpage
   @brief
   @author Bruno da Silva Belo
*/

#include <GL/glew.h>
#include <stdio.h>
#include <exception>
#include "window.h"
#include "shader.h"
#include "mesh.h"
#include "vertex.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include <vector>
#include "entity.h"
#include <glm/gtx/string_cast.hpp>
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"

using namespace std;

int main() {
  try {
    Window::init("OpenGL", 800, 600);

    vector<Vertex> vertices = {
      Vertex(glm::vec3(-1.f, -1.f, 0.5773f), glm::vec2(0.f, 0.f)),
      Vertex(glm::vec3(0.f, -1.f, -1.15475f), glm::vec2(0.5f, 0.f)),
      Vertex(glm::vec3(1.f, -1.f, 0.5773f), glm::vec2(1.f, 0.f)),
      Vertex(glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.5f, 1.f)),
    };

    vector<unsigned> indices = {0, 3, 1,
                                1, 3, 2,
                                2, 3, 0,
                                0, 1, 2};

    vector<Vertex> vertices1 = {
      Vertex({-1.f,1.f,1.f},{0.f, 1.f}), // 0
      Vertex({-1.f,-1.f,1.f},{0.f,0.f}), // 1
      Vertex({1.f,-1.f,1.f},{1.f,0.f}),  // 2
      Vertex({1.f,1.f,1.f},{1.f,1.f}),   // 3
      Vertex({-1.f,1.f,-1.f},{1.f, 0.f}),// 4
      Vertex({-1.f,-1.f,-1.f},{1.f,1.f}),// 5
      Vertex({1.f,-1.f,-1.f},{0.f,1.f}), // 6
      Vertex({1.f,1.f,-1.f},{0.f,0.f}),  // 7
    };

    vector<unsigned> indices1 = {
      0,1,2,
      2,3,0,
      5,4,7,
      7,6,5,
      4,0,3,
      3,7,4,
      4,5,1,
      1,0,4,
      3,2,6,
      6,7,3,
      2,1,5,
      5,6,2,
    };

    Mesh mesh(vertices, indices);
    Mesh mesh2(vertices1, indices1, false);
    Shader shader("../src/glsl/basicShader");

    Transform transform;

    Camera camera(glm::vec3(0.f, 0.f, 3.f), 70.f, Window::aspect(), 0.1f, 100.f);

    Entity light(Transform({2.f, 2.f, 2.f},{}, glm::vec3(0.2f)), mesh2, "../res/textures/container2.png");
    Shader lightShader("../src/glsl/lightShader");
    lightShader.bind();
    lightShader.update(Shader::PROJECTION, camera.getProjection());
    DirectionalLight dLight({0.f, 0.f, -1.f});
    PointLight pLight(light.getPosition());
    SpotLight sLight(camera.getPosition(), camera.getForward());
    dLight.init(shader);
    pLight.init(shader);
    sLight.init(shader);
    transform.applyScale({0.5f,0.5f, 0.5f});
    Mesh mesh3("../res/models/spider.obj");
    Entity monkey(transform, mesh2, "../res/textures/container2.png");
    monkey.addTexture("../res/textures/container2_specular.png");

    shader.bind();
    shader.update(Shader::PROJECTION, camera.getProjection());
    shader.update("material.diffuse", 0);
    shader.update("material.specular", 1);
    shader.update("material.shininess", 32.f);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    GLfloat counter = 0.f;

    Window::startTimer();
    while (!Window::isClosed()) {
      glfwPollEvents();
      Window::clear();

      camera.update(Window::deltaTime(), Window::getKey(), Window::getMousePosition()
                    , Window::getMouseButton());

      for (unsigned i = 0; i < 10; ++i) {
        monkey.applyTranslate(cubePositions[i]);
        GLfloat angle = 20.f * i;
        monkey.applyRotation(glm::vec3(1.f * angle, 0.3f * angle, 0.5f * angle));
        monkey.draw(shader, camera);
      }
      shader.update("viewPosition", camera.getPosition());

      dLight.bind(shader);
      sLight.update(camera.getPosition(), SpotLight::POSITION);
      sLight.update(camera.getForward(), SpotLight::DIRECTION);
      sLight.bind(shader);
      light.applyTranslate(glm::vec3(2.f,2.f,2.f) + glm::vec3(sin(counter) * 10, 0.f,0.f));
      pLight.update(light.getPosition());
      pLight.bind(shader);
      light.draw(lightShader, camera);
      Window::update();

      counter += 0.01f;
    }
    //printf("Directional\n%s\nPoint\n%s\nSpot\n%s\n", dLight.to_string().c_str(), pLight.to_string().c_str(),
    //       sLight.to_string().c_str());
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s\n", e.what());
  }
  Window::close();

  return 0;
}
