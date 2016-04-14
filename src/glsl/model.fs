#version 330 core

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  float shininess;
};

struct BaseLight {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct DirectionalLight {
  vec3 direction;

  BaseLight base;
};

struct Attenuation {
  float constant;
  float linear;
  float quadratic;
};

struct PointLight {
  vec3 position;

  Attenuation attenuation;
  BaseLight base;
};

struct SpotLight {
  bool isOn;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  PointLight base;
};

in VS_OUT {
 vec2 texCoord;
 vec3 fragPosition;
 vec3 normal;
} fs_in;

out vec4 color;

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform int numPointLight;
uniform PointLight pointLight[10];
uniform SpotLight spotLight;
uniform samplerCube skybox;

const float near = 1.f;
const float far = 100.f;

vec3 calcBaseLight(BaseLight light, vec3 lightDirection, vec3 normal);
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal);
vec3 calcPointLight(PointLight light, vec3 normal);
vec3 calcSpotLight(SpotLight light, vec3 normal);
float linearizeDepth(float depth);

vec3 calcBaseLight(BaseLight light, vec3 lightDirection, vec3 normal) {
  vec3 ambient = light.ambient *
    vec3(texture(material.texture_diffuse1, fs_in.texCoord));

  vec3 diffuse = vec3(0.f);
  vec3 specular = vec3(0.f);

  float diff = dot(normal, -lightDirection);
  if (diff > 0) {
    diffuse = diff * (light.diffuse
                      * vec3(texture(material.texture_diffuse1, fs_in.texCoord)));

    vec3 viewDirection = normalize(viewPosition - fs_in.fragPosition);
    vec3 reflectDirection = normalize(reflect(lightDirection, normal));
    float spec = dot(viewDirection, reflectDirection);
    if (spec > 0) {
      spec = pow(spec, material.shininess);
      specular = spec * (light.specular
                         * vec3(texture(material.texture_specular1, fs_in.texCoord)));
    }
  }

  return (ambient + diffuse + specular);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal) {
  return calcBaseLight(light.base, light.direction, normal);
}

vec3 calcPointLight(PointLight light, vec3 normal) {
  vec3 lightDirection = fs_in.fragPosition - light.position;
  float dist = length(lightDirection);
  lightDirection = normalize(lightDirection);

  vec3 result = calcBaseLight(light.base, lightDirection, normal);
  float attenuation = 1.f / (light.attenuation.constant
                             + light.attenuation.linear * dist
                             + light.attenuation.quadratic * dist * dist);

  return attenuation * result;
}

vec3 calcSpotLight(SpotLight light, vec3 normal) {
  vec3 lightDirection = normalize(fs_in.fragPosition - light.base.position);
  float theta = dot(lightDirection, light.direction);

  if (theta < light.cutOff)
    return vec3(0.f);

  vec3 result = calcPointLight(light.base, normal);
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);

  return intensity * result;
}

float linearizeDepth(float depth) {
  float z = depth * 2.f - 1.f;
  return (2.f * near * far) / (far + near - z * (far - near));
}

void main() {
  vec3 normal = normalize(fs_in.normal);

  vec3 result = vec3(0.f);
  result += calcDirectionalLight(directionalLight, normal);

  for (int i = 0; i < numPointLight; ++i)
    result += calcPointLight(pointLight[i], normal);

  if (spotLight.isOn)
    result += calcSpotLight(spotLight, normal);

  color = vec4(result, 1.f);
}
