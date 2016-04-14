#version 330 core

#define INVERSION 0
#define GRAYSCALE 0
#define SEPIA 0
#define KERNEL 0
#define SHARPEN

in vec2 texCoord0;

out vec4 color;

uniform sampler2D screenTexture;

void main() {
  color = texture(screenTexture, texCoord0);
#if INVERSION
  color = vec4(vec3( 1 - color), 1.f);
#endif
#if GRAYSCALE
  float average = 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
  color = vec4(vec3(average), 1.f);
#endif
#if SEPIA
  vec4 outcolor = color;
  outcolor.r = (color.r * 0.393f) + (color.g * 0.769f) + (color.b * 0.189f);
  outcolor.g = (color.r * 0.349f) + (color.g * 0.686f) + (color.b * 0.168f);
  outcolor.b = (color.r * 0.272f) + (color.g * 0.534f) + (color.b * 0.131f);
  color = outcolor;
#endif
#if KERNEL
  const float offset = 1.f/300.f;
  vec2 offsets[9] = vec2[](
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

  float kernel[9] = float[] (
#ifdef SHARPEN
                             0,-1.f,0,
                             -1.f,5.f,-1.f,
                             0,-1.f,0
#endif
#ifdef GAUSSIANBLUR
                             1.f/16.f,2.f/16.f,1.f/16.f,
                             2.f/16.f,4.f/16.f,2.f/16.f,
                             1.f/16.f,2.f/16.f,1.f/16.f
#endif
#ifdef BOXBLUR
                             1.f/9.f,1.f/9.f,1.f/9.f,
                             1.f/9.f,1.f/9.f,1.f/9.f,
                             1.f/9.f,1.f/9.f,1.f/9.f
#endif
#ifdef EDGE
                             1.f,1.f,1.f,
                             1.f,-7.f,1.f,
                             1.f,1.f,1.f
#endif
#ifdef EMBOSS
                             -2.f,-1.f,0,
                             -1.f,1.f,1.f,
                             0,1.f,2.f
#endif
#ifdef LEFTSOBEL
                             1.f,0,-1.f,
                             2.f,0,-2.f,
                             1.f,0,-1.f
#endif
#ifdef RIGHTSOBEL
                             -1.f,0,1.f,
                             -2.f,0,2.f,
                             -1.f,0,1.f
#endif
#ifdef TOPSOBEL
                             1.f,2.f,1.f,
                             0,0,0,
                             -1.f,-2.f,-1.f
#endif
#ifdef BOTTOMSOBEL
                             -1.f,-2.f,-1.f,
                             0,0,0,
                             1.f,2.f,1.f
#endif
#ifdef IDENTITY
                             0,0,0,
                             0,1.f,0,
                             0,0,0
#endif
#ifdef CUSTOM
                             1.f,1.f,1.f,
                             1.f,-8.f,1.f,
                             1.f,1.f,1.f
#endif
                             );
  vec3 sampleTex[9];
  for (int i = 0; i < 9; ++i)
    sampleTex[i] = vec3(texture(screenTexture, texCoord0 + offsets[i]));

  vec3 col = vec3(0.f);
  for (int i = 0; i < 9; ++i)
    col += sampleTex[i] * kernel[i];

  color = vec4(col, 1.f);
#endif
}
