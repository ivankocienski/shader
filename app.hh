
#pragma once

#include "mesh.hh"
#include "shader.hh"
#include "font.hh"

class App {
private:

  Mesh   m_mesh;
  Shader m_shader;
  Font   m_font;

  float  m_angle; 

  ShaderUniformVar m_light_vector;
  

public:

  App();

  void init();
  void tick();
};

