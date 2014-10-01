
#include "app.hh"

static float vertex_data[3][3] = {
  {  0.0,  1.0, 1.0  },
  { -1.0, -1.0, 1.0  },
  {  1.0, -1.0, 1.0  }
};

App::App() { 
}

void App::init() {
  
  m_mesh.load_from( 3, (float *)vertex_data ); 
  m_mesh.set_shader_attribute(0);

}

void App::tick() {

}

