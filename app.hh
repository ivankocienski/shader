
#pragma once

#include "mesh.hh"

class App {
private:

  Mesh m_mesh;

public:

  App();

  void init();
  void tick();
};

