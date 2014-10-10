
#pragma once

#include <GL/gl.h>

#include "shader.hh"

class Background {
private:

  Shader m_shader;

  GLuint m_texture;
  GLuint m_vbo;
  GLuint m_vao;

  void init_shader();
  void init_vbo();
  void init_texture();
  
public:

  Background();

  void init();
  void draw();
};

