#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 texCoord0;
out vec3 normal0;

uniform mat4 world;
uniform mat4 perspective;
uniform mat4 camera;

void main() {
  gl_Position = perspective * camera * world * vec4(position, 1.0);
  texCoord0 = texCoord;
  normal0 = (perspective * vec4(normal, 0.0)).xyz;
}
