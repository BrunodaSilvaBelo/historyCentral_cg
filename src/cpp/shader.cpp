#include "shader.h"
#include <fstream>
#include <stdio.h>
#include "transform.h"
#include "camera.h"
#include <glm/gtc/type_ptr.hpp>

using namespace std;

string loadShader(const string &file);
void checkShaderError(GLuint shader, GLuint flag, GLboolean isProgram,
                      const string &errorMessage);
GLuint createShader(const string &text, GLenum shaderType);

Shader::Shader(const string &file) {
  program = glCreateProgram();
  shaders[0] = createShader(loadShader(file + ".vs"), GL_VERTEX_SHADER);
  shaders[1] = createShader(loadShader(file + ".fs"), GL_FRAGMENT_SHADER);

  for (unsigned i = 0; i < NUM_SHADERS; ++i)
    glAttachShader(program, shaders[i]);

  glBindAttribLocation(program, 0, "position");
  glBindAttribLocation(program, 1, "texCoord");

  glLinkProgram(program);
  checkShaderError(program, GL_LINK_STATUS, true,
                   "Error: Program linking failed:");

  glValidateProgram(program);
  checkShaderError(program, GL_VALIDATE_STATUS, true,
                   "Error: Program linking failed:");

  uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform");
}

Shader::~Shader() {
  for (unsigned i = 0; i < NUM_SHADERS; ++i) {
    glDetachShader(program, shaders[i]);
    glDeleteShader(shaders[i]);
  }
  glDeleteProgram(program);
}

void Shader::bind() {
  glUseProgram(program);
}

void Shader::update(const Transform &transform, const Camera &camera) {
  glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GL_FALSE,
                     glm::value_ptr(transform.getModel()));
}

string loadShader(const string &file) {
  ifstream stream;
  stream.open(file.c_str());

  string output;
  string line;

  if (stream.is_open())
    while (stream.good()) {
      getline(stream, line);
      output.append(line + "\n");
    }
  else
    fprintf(stderr, "Unable to load shader: %s!\n", file.c_str());

  return output;
}

void checkShaderError(GLuint shader, GLuint flag, GLboolean isProgram,
                      const string &errorMessage) {
  GLint success = 0;
  GLchar error[1024] = { 0 };

  (isProgram) ? glGetProgramiv(shader, flag, &success)
    : glGetShaderiv(shader, flag, &success);

  if (!success) {
    (isProgram) ? glGetProgramInfoLog(shader, sizeof(error), NULL, error)
      : glGetShaderInfoLog(shader, sizeof(error), NULL, error);
    fprintf(stderr, "%s: %s!\n", errorMessage.c_str(), error);
  }
}

GLuint createShader(const string &text, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);
  if (!shader)
    fprintf(stderr, "Error: Shader creation failed!\n");

  const GLchar *shaderSourceStrings[1];
  shaderSourceStrings[0] = text.c_str();

  GLint shaderSourceStringLengths[1];
  shaderSourceStringLengths[0] = static_cast<GLint>(text.length());

  glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
  glCompileShader(shader);
  checkShaderError(shader, GL_COMPILE_STATUS, false,
                   "Error: Shader compilation failed: ");
  return shader;
}
