#include <iostream>
#include "Shader.h"

Shader::Shader(void)
{
  g_shProg = glCreateProgram();
}

Shader::Shader(const char* vert_src, const char* frag_src)
{
  g_shProg = glCreateProgram();
  addVertex(vert_src);
  addFragment(frag_src);
  link();
}

Shader::~Shader(void)
{
  glDeleteProgram(g_shProg);
}

void Shader::addVertex(const char* vert_src)
{
  char shErr[1024];
  int errlen;
  GLint res;

  // Generate some IDs for our shader programs
  g_shVert = glCreateShader(GL_VERTEX_SHADER);

  // Assign our above shader source code to these IDs
  glShaderSource(g_shVert, 1, &vert_src, nullptr);

  // Attempt to compile the source code
  glCompileShader(g_shVert);

  // check if compilation was successful
  glGetShaderiv(g_shVert, GL_COMPILE_STATUS, &res);
  if (GL_FALSE == res){
    fprintf(stderr, "Failed to compile vertex shader\n");
    glGetShaderInfoLog(g_shVert, 1024, &errlen, shErr);
    printf("%s\n", shErr);
    return;
  }

  // Attach these shaders to the shader program
  glAttachShader(g_shProg, g_shVert);

  // flag the shaders to be deleted when the shader program is deleted
  glDeleteShader(g_shVert);
}

GLuint Shader::getMVPLocation(void)
{
  return mvp_uni;
}

void Shader::addFragment(const char* frag_src)
{
  char shErr[1024];
  int errlen;
  GLint res;

  // Generate some IDs for our shader programs
  g_shFrag = glCreateShader(GL_FRAGMENT_SHADER);

  // Assign our above shader source code to these IDs
  glShaderSource(g_shFrag, 1, &frag_src, nullptr);

  // Attempt to compile the source code
  glCompileShader(g_shFrag);

  // check if compilation was successful
  glGetShaderiv(g_shFrag, GL_COMPILE_STATUS, &res);
  if (GL_FALSE == res){
    fprintf(stderr, "Failed to compile fragment shader\n");
    glGetShaderInfoLog(g_shFrag, 1024, &errlen, shErr);
    printf("%s\n", shErr);
    return;
  }

  // Attach these shaders to the shader program
  glAttachShader(g_shProg, g_shFrag);

  // flag the shaders to be deleted when the shader program is deleted
  glDeleteShader(g_shFrag);
}

void Shader::link(void)
{
  GLint res;
  // Link the shaders
  glLinkProgram(g_shProg);
  glGetProgramiv(g_shProg, GL_LINK_STATUS, &res);

  if (GL_FALSE == res)
    fprintf(stderr, "Failed to link shader program\n");
}

GLuint Shader::getProgram(void)
{
  return g_shProg;
}

void Shader::createUniform(const char* uniform_name)
{
  glUseProgram(g_shProg);
  uniform_location[uniform_name] = glGetUniformLocation(g_shProg, uniform_name);
}

void Shader::setUniformMat4(const char* uniform_name, glm::mat4 & value)
{
  glUseProgram(g_shProg);
  glUniformMatrix4fv(uniform_location[uniform_name], 1, GL_FALSE, &value[0][0]);
}
