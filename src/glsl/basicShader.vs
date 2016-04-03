#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 normal0;
out vec2 texCoord0;
out vec3 fragPosition;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.f);
  fragPosition = vec3(model * vec4(position, 1.f));
  normal0 = mat3(transpose(inverse(model))) * normal;
  texCoord0 = texCoord;
}
