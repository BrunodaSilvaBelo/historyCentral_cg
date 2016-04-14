#version 330 core

layout (location = 0) in vec3 position;

layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};

out vec3 texCoord0;

uniform mat4 skyview;

void main() {
  vec4 pos = projection * skyview * vec4(position, 1.f);
  gl_Position = pos.xyww;
  texCoord0 = position;
}
