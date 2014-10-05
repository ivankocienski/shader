
#pragma once

#include <GL/gl.h>
#include <vector>

#include "mesh_loader.hh"

class Mesh {
private:

  MeshLoader::node_p m_vertex_data;

  GLuint m_vao_handle;

  // one of these is the VBO for VN
  // two is index buffer
  GLuint m_objects[2];

  int m_num_vertices;
  int m_num_indices;

  GLuint m_shader_attribute;

public:

  Mesh();
  ~Mesh();

  void load_from( const MeshLoader& );

  void set_shader_attribute( GLuint );

  GLuint object_handle();
  GLuint vertex_object_handle();
  GLuint index_object_handle();

  void bind();
  void draw();
  void bind_and_draw();


};

