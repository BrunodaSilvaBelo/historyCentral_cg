#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};

out VS_OUT {
  vec2 texCoord;
  vec3 fragPosition;
  vec3 normal;
} vs_out;

uniform mat4 model;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.f);
  vs_out.fragPosition = vec3(model * vec4(position, 1.f));
  vs_out.normal = mat3(transpose(inverse(model))) * normal;
  vs_out.texCoord = texCoord;
}
