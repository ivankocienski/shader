
#pragma once

#include <GL/gl.h>
#include "mesh_loader.hh"

class Mesh {
private:

  GLuint m_vao_handle;
  GLuint m_vbo_handle;
  int m_num_vertices;
  GLuint m_shader_attribute;

public:

  Mesh();
  ~Mesh();

  bool load_from( const MeshLoader& );
  void set_shader_attribute( GLuint );

  GLuint vao_handle();
  GLuint vbo_handle();

  void bind();
  void draw();
  void bind_and_draw();


};

