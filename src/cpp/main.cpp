/**
   @mainpage
   @brief
   @author Bruno da Silva Belo
   @bug Luz só funciona se existir uma directional light
   @bug Directional light parou de funcionar
*/

#include <GL/glew.h>
#include <exception>
#include "buffer.h"
#include "clear.h"
#include "normal.h"
#include "render.h"
#include "scene.h"

namespace aspects {
  typedef components::BaseTexture<components::MsaaTexture> MsaaTexture;
  typedef components::BaseTexture<components::Normal> Texture;
}

namespace modules {
  typedef components::Buffer<components::MsaaRender,
			     aspects::MsaaTexture> Msaa;
  typedef components::Buffer<components::Render,
			     aspects::Texture> Normal;
}

namespace configuration {
  typedef components::Scene<modules::Msaa,
			    modules::Normal,
			    components::SkyboxScene<>> MainScene;
}

using namespace std;

int main() {
  try {
    Window::init("OpenGL", 800, 600);
    configuration::MainScene scene;

    Window::startTimer();
    while (!Window::isClosed()) {
      glfwPollEvents();

      scene.run();
      Window::update();
    }
  } catch(const std::exception &e) {
    fprintf(stderr, "Error: %s\n", e.what());
  }
  Window::close();

  return 0;
}
