#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirectionalLight {
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

struct SpotLight {
  bool isOn;
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

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform int numPointLight;
uniform PointLight pointLight[10];
uniform SpotLight spotLight;

float calcDiff(vec3 normal, vec3 lightDirection);
float calcSpec(vec3 normal, vec3 viewDirection);
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);

float calcDiff(vec3 normal, vec3 lightDirection) {
  return max(dot(normal, lightDirection), 0.f);
}

float calcSpec(vec3 normal, vec3 lightDirection, vec3 viewDirection) {
  vec3 reflectDirection = reflect(-lightDirection, normal);
  return pow(max(dot(viewDirection, reflectDirection), 0.f), material.shininess);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
  vec3 lightDirection = normalize(-light.direction);
  // diffuse
  float diff = calcDiff(normal, lightDirection);
  //specular
  float spec = calcSpec(normal, lightDirection, viewDirection);

  vec3 ambient = light.ambient * vec3(texture(material.diffuse,texCoord0));
  vec3 diffuse = diff * (light.diffuse * vec3(texture(material.diffuse, texCoord0)));
  vec3 specular = spec * (light.specular * vec3(texture(material.specular, texCoord0)));

  return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - fragPosition);
  // diffuse
  float diff = calcDiff(normal, lightDirection);
  // specular
  float spec = calcSpec(normal, lightDirection, viewDirection);
  // Attenuation
  float dist = length(light.position - fragPosition);
  float attenuation = 1.f / (light.constant + light.linear * dist
                             + light.quadratic * dist * dist);

  vec3 ambient = (attenuation)
    * (light.ambient * vec3(texture(material.diffuse, texCoord0)));
  vec3 diffuse = (attenuation * diff)
    * (light.diffuse * vec3(texture(material.diffuse, texCoord0)));
  vec3 specular = (attenuation * spec)
    * (light.specular * vec3(texture(material.specular, texCoord0)));

  return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPosition,
                   vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - fragPosition);
  // diffuse
  float diff = calcDiff(normal, lightDirection);
  // specular
  float spec = calcSpec(normal, lightDirection, viewDirection);

  float theta = dot(lightDirection, normalize(-light.direction));
  
  if (theta < light.cutOff) {
    // Attenuation
    float dist = length(light.position - fragPosition);
    float attenuation = 1.f / (light.constant + light.linear * dist
                               + light.quadratic * dist * dist);

    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);

    vec3 ambient = (attenuation)
      * (light.ambient * vec3(texture(material.diffuse, texCoord0)));
    vec3 diffuse = (attenuation * intensity * diff)
      * (light.diffuse * vec3(texture(material.diffuse, texCoord0)));
    vec3 specular = (attenuation * intensity * spec)
      * (light.specular * vec3(texture(material.specular, texCoord0)));

    return vec3(ambient + specular + diffuse);
  } else
    return vec3(0.f);
}

void main() {
  vec3 norm = normalize(normal0);
  vec3 viewDirection = normalize(viewPosition - fragPosition);

  vec3 result = calcDirectionalLight(directionalLight, norm, viewDirection);

  for (int i = 0; i < numPointLight; ++i)
    result += calcPointLight(pointLight[i], norm, fragPosition, viewDirection);

  if (spotLight.isOn)
    result += calcSpotLight(spotLight, norm, fragPosition, viewDirection);

  color = vec4(result, 1.f);
}
