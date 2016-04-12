#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec2 texCoord0;

void main() {
  gl_Position = vec4(position.x, position.y, 0.f, 1.f);
  texCoord0 = texCoord;
}
