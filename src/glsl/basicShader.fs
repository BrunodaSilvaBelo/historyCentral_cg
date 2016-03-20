#version 330 core

in vec3 normal0;
in vec3 fragPosition;
in vec2 texCoord0;

out vec4 color;

uniform sampler2D sampler;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main() {
  // ambient
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse
  vec3 norm = normalize(normal0);
  vec3 lightDirection = normalize(lightPosition - fragPosition);
  float diff = max(dot(norm, lightDirection), 0.f);
  vec3 diffuse = diff * lightColor;

  // specular
  float specularStrength = 0.5f;
  vec3 viewDirection = normalize(viewPosition - fragPosition);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.f), 32);
  vec3 specular = specularStrength * spec * lightColor;

  color = texture2D(sampler, texCoord0) * vec4((ambient + diffuse + specular) * objectColor, 1.f);
}
