
#pragma once

#include "mesh.hh"
#include "shader.hh"

class Bunny {
private:

  Mesh   m_mesh;
  Shader m_shader;
  Shader m_shadow_shader;

  ShaderUniformVar m_light_vector;

public:

  Bunny();

  void init();
  void draw(float);
};

