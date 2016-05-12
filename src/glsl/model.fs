#version 330 core

#define NORMAL

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
  vec4 lightSpacePos;
} fs_in;

out vec4 color;

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform int numPointLight;
uniform PointLight pointLight[10];
uniform SpotLight spotLight;
uniform samplerCube skybox;
uniform sampler2D shadowMap;

const float near = 1.f;
const float far = 100.f;

vec3 calcBaseLight(BaseLight light, vec3 lightDirection, vec3 normal,
		   float shadowFactor);
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal);
vec3 calcPointLight(PointLight light, vec3 normal, vec4 lightSpacePos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec4 lightSpacePos);
float linearizeDepth(float depth);
float calcShadowFactor(vec4 fragPosLightSpace);

#ifdef NORMAL
vec3 calcBaseLight(BaseLight light, vec3 lightDirection, vec3 normal,
		   float shadowFactor) {
  vec3 ambient = light.ambient *
    vec3(texture(material.texture_diffuse1, fs_in.texCoord));

  vec3 diffuse = vec3(0.f);
  vec3 specular = vec3(0.f);

  float diff = dot(normal, -lightDirection);
  if (diff > 0) {
    diffuse = (diff) * (light.diffuse
			* vec3(texture(material.texture_diffuse1, fs_in.texCoord)));

    vec3 viewDirection = normalize(viewPosition - fs_in.fragPosition);
    vec3 reflectDirection = normalize(reflect(lightDirection, normal));
    float spec = dot(viewDirection, reflectDirection);
    if (spec > 0) {
      spec = pow(spec, material.shininess);
      specular = (spec) * (light.specular
			   * vec3(texture(material.texture_specular1, fs_in.texCoord)));
    }
  }
  return (ambient + 1.f * (diffuse + specular));
}
#endif
#ifdef CEL
vec3 calcBaseLight(BaseLight light, vec3 lightDirection, vec3 normal,
		   float shadowFactor) {
  vec3 per;
  float diff = dot(-lightDirection, normal);
  if (diff > 0.95f)
    per = vec3(0.8f, 0.5f, 0.5f);
  else if (diff > 0.5)
    per = vec3(0.6f, 0.4f, 0.4f);
  else if (diff > 0.25)
    per = vec3(0.4f, 0.2f, 0.2f);
  else
    per = vec3(0.2f, 0.1f, 0.1f);

  return per;
}
#endif
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal) {
  return calcBaseLight(light.base, light.direction, normal, 1.f);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec4 lightSpacePos) {
  vec3 lightDirection = fs_in.fragPosition - light.position;
  float dist = length(lightDirection);
  lightDirection = normalize(lightDirection);
  float shadowFactor = calcShadowFactor(lightSpacePos);

  vec3 result = calcBaseLight(light.base, lightDirection, normal, shadowFactor);
  float attenuation = 1.f / (light.attenuation.constant
                             + light.attenuation.linear * dist
                             + light.attenuation.quadratic * dist * dist);

  return attenuation * result;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec4 lightSpacePos) {
  vec3 lightDirection = normalize(fs_in.fragPosition - light.base.position);
  float theta = dot(lightDirection, light.direction);

  if (theta < light.cutOff)
    return vec3(0.f);

  vec3 result = calcPointLight(light.base, normal, lightSpacePos);
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);

  return intensity * result;
}

float linearizeDepth(float depth) {
  float z = depth * 2.f - 1.f;
  return (2.f * near * far) / (far + near - z * (far - near));
}

float calcShadowFactor(vec4 lightSpacePos) {
  vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
  vec2 uvCoords;
  uvCoords.x = 0.5f * projCoords.x + 0.5f;
  uvCoords.y = 0.5f * projCoords.y + 0.5f;
  float z = 0.5f * projCoords.z + 0.5f;
  float depth = texture(shadowMap, uvCoords).z;

  float shadow = 1.f;
  if (lightSpacePos.w > 0.f)
    shadow = depth < projCoords.z + 0.0005f ? 0.5f : 1.f;

  return shadow;
}

void main() {
  vec3 normal = normalize(fs_in.normal);

  vec3 result = vec3(0.f);
  result += calcDirectionalLight(directionalLight, normal);

  for (int i = 0; i < numPointLight; ++i)
    result += calcPointLight(pointLight[i], normal, fs_in.lightSpacePos);
  if (!spotLight.isOn)
    result += calcSpotLight(spotLight, normal, fs_in.lightSpacePos);

  color = vec4(result, 1.f);
}
