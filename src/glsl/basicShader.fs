#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirectionLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Light {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 normal0;
in vec3 fragPosition;
in vec2 texCoord0;

out vec4 color;

uniform float test[MAX];
uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main() {
  // ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord0));

  // diffuse
  vec3 norm = normalize(normal0);
  vec3 lightDirection = normalize(light.direction - fragPosition);
  float diff = max(dot(norm, lightDirection), 0.f);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord0));

  // specular
  vec3 viewDirection = normalize(viewPosition - fragPosition);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.f),
                   material.shininess);
  vec3 specular = light.specular * spec
    * vec3(texture(material.specular, texCoord0));

  // Spotlight (soft edges)
  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = (light.cutOff - light.outerCutOff);
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);

  diffuse *= intensity;
  specular *= intensity;

  // Attenuation
  float dist = length(light.position - fragPosition);
  float attenuation = 1.f / (light.constant + light.linear * dist
                             + light.quadratic * (dist * dist));
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  color = vec4(ambient + diffuse + specular, 1.f);
}
