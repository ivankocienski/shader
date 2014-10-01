
#pragma once

#include "mesh.hh"
#include "shader.hh"

class App {
private:

  Mesh   m_mesh;
  Shader m_shader;

public:

  App();

  void init();
  void tick();
};

