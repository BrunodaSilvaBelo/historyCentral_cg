#version 330 core

// POST_PROCESSING
#define INVERSION 0
#define GRAYSCALE 0
#define SEPIA 0
#define KERNEL 0
#define SOBEL                        // SHARPEN, BLUR, EDGE, SOBEL, LEFT_SOBEL

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

#if !KERNEL
vec3 calcBaseLight(BaseLight light, vec3 lightDirection, vec3 normal);
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal);
vec3 calcPointLight(PointLight light, vec3 normal);
vec3 calcSpotLight(SpotLight light, vec3 normal);

vec3 calcBaseLight(BaseLight light, vec3 lightDirection, vec3 normal) {
  vec3 ambient = light.ambient *
    vec3(texture(material.texture_diffuse1, texCoord0));

  vec3 diffuse = vec3(0.f);
  vec3 specular = vec3(0.f);

  float diff = dot(normal, -lightDirection);
  if (diff > 0) {
    diffuse = diff * (light.diffuse
                      * vec3(texture(material.texture_diffuse1, texCoord0)));

    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectDirection = normalize(reflect(lightDirection, normal));
    float spec = dot(viewDirection, reflectDirection);
    if (spec > 0) {
      spec = pow(spec, material.shininess);
      specular = spec * (light.specular
                         * vec3(texture(material.texture_specular1, texCoord0)));
    }
  }

  return (ambient + diffuse + specular);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal) {
  return calcBaseLight(light.base, light.direction, normal);
}

vec3 calcPointLight(PointLight light, vec3 normal) {
  vec3 lightDirection = fragPosition - light.position;
  float dist = length(lightDirection);
  lightDirection = normalize(lightDirection);

  vec3 result = calcBaseLight(light.base, lightDirection, normal);
  float attenuation = 1.f / (light.attenuation.constant
                             + light.attenuation.linear * dist
                             + light.attenuation.quadratic * dist * dist);

  return attenuation * result;
}

vec3 calcSpotLight(SpotLight light, vec3 normal) {
  vec3 lightDirection = normalize(fragPosition - light.base.position);
  float theta = dot(lightDirection, light.direction);

  if (theta < light.cutOff)
    return vec3(0.f);

  vec3 result = calcPointLight(light.base, normal);
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);

  return intensity * result;
}

void main() {
  vec3 normal = normalize(normal0);

  vec3 result = vec3(0.f);
  result += calcDirectionalLight(directionalLight, normal);

  for (int i = 0; i < numPointLight; ++i)
    result += calcPointLight(pointLight[i], normal);

  if (spotLight.isOn)
    result += calcSpotLight(spotLight, normal);

  color = vec4(result, 1.f);

#if GRAYSCALE
  float avg = 0.2126f * color.r + 0.7152 * color.g + 0.0722 * color.b;
  color = vec4(vec3(avg), 1.f);
#endif
#if INVERSION
  color = vec4(1-result, 1.f);
#endif
#if SEPIA
  vec4 sepiaColor = vec4(
                         clamp(color.r * 0.393f + color.g * 0.769f + color.b * 0.189f, 0.f, 1.f),
                         clamp(color.r * 0.349f + color.g * 0.686f + color.b * 0.168f, 0.f, 1.f),
                         clamp(color.r * 0.272f + color.g * 0.534f + color.b * 0.131f, 0.f, 1.f),
                         color.a);
  color = mix(color, sepiaColor, 1.f);
#endif
}
#else
vec3 calcBaseLight(BaseLight light, vec3 lightDirection, vec3 normal, vec2 offset);
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec2 offset);
vec3 calcPointLight(PointLight light, vec3 normal, vec2 offset);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec2 offset);

vec3 calcBaseLight(BaseLight light, vec3 lightDirection, vec3 normal, vec2 offset) {
  vec3 ambient = light.ambient *
    vec3(texture(material.texture_diffuse1, texCoord0.st + offset));

  vec3 diffuse = vec3(0.f);
  vec3 specular = vec3(0.f);

  float diff = dot(normal, -lightDirection);
  if (diff > 0) {
    diffuse = diff * (light.diffuse
                      * vec3(texture(material.texture_diffuse1,
                                     texCoord0.st + offset)));

    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectDirection = normalize(reflect(lightDirection, normal));
    float spec = dot(viewDirection, reflectDirection);
    if (spec > 0) {
      spec = pow(spec, material.shininess);
      specular = spec * (light.specular
                         * vec3(texture(material.texture_specular1,
                                        texCoord0.st + offset)));
    }
  }

  return (ambient + diffuse + specular);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec2 offset) {
  return calcBaseLight(light.base, light.direction, normal, offset);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec2 offset) {
  vec3 lightDirection = fragPosition - light.position;
  float dist = length(lightDirection);
  lightDirection = normalize(lightDirection);

  vec3 result = calcBaseLight(light.base, lightDirection, normal, offset);
  float attenuation = 1.f / (light.attenuation.constant
                             + light.attenuation.linear * dist
                             + light.attenuation.quadratic * dist * dist);

  return attenuation * result;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec2 offset) {
  vec3 lightDirection = normalize(fragPosition - light.base.position);
  float theta = dot(lightDirection, light.direction);

  if (theta < light.cutOff)
    return vec3(0.f);

  vec3 result = calcPointLight(light.base, normal, offset);
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);

  return intensity * result;
}

void main() {
  vec3 normal = normalize(normal0);

  const float offset = 1.f/300.f;
  const vec2 offsets[9] = vec2[](
                                 vec2(-offset, offset),  // top-left
                                 vec2(0.0f,    offset),  // top-center
                                 vec2(offset,  offset),  // top-right
                                 vec2(-offset, 0.0f),    // center-left
                                 vec2(0.0f,    0.0f),    // center-center
                                 vec2(offset,  0.0f),    // center-right
                                 vec2(-offset, -offset), // bottom-left
                                 vec2(0.0f,    -offset), // bottom-center
                                 vec2(offset,  -offset)  // bottom-right
                                 );

#ifdef SHARPEN
  float kernel[9] = float[](
                            -1, -1, -1,
                            -1,  5, -1,
                            -1, -1, -1
                            );
#endif
#ifdef BLUR
  float kernel[9] = float[](
                            1.f/16.f, 2.f/16.f, 1.f/16.f,
                            2.f/16.f, 4.f/16.f, 2.f/16.f,
                            1.f/16.f, 2.f/16.f, 1.f/16.f
                            );
#endif
#ifdef EDGE
  float kernel[9] = float[](
                            1, 1, 1,
                            1, -8, 1,
                            1, 1, 1
                            );
#endif
#ifdef LEFT_SOBEL
  float kernel[9] = float[](
                            1, 0, -1,
                            2, 0, -2,
                            1, 0, -1
                            );
#endif
#ifdef SOBEL
  float kernel[9] = float[](
                            -2, -1, 0,
                            -1, 1, 1,
                            0, 1, 2
                            );
#endif
  vec3 sampleTex[9];
  for (int i = 0; i < 9; ++i)
    sampleTex[i] = calcDirectionalLight(directionalLight, normal, offsets[i]);

  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < numPointLight; ++j)
      sampleTex[i] += calcPointLight(pointLight[j], normal, offsets[i]);

  for (int i = 0; i < 9; ++i)
    sampleTex[i] += calcSpotLight(spotLight, normal, offsets[i]);

  vec3 col = vec3(0.f);
  for (int i = 0; i < 9; ++i)
    col += sampleTex[i] * kernel[i];

  color = vec4(col, 1.f);
}
#endif
