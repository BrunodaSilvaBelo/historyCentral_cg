#version 330 core

layout (location = 0) in vec3 position;

layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};

out vec3 texCoord0;

void main() {
  mat4 skyview = mat4(mat3(view));
  vec4 pos = projection * skyview * vec4(position, 1.f);
  gl_Position = pos.xyww;
  texCoord0 = position;
}
