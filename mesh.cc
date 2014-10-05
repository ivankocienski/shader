
#include <iostream>
#include <cstring>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/gl.h>

#include "gl_error.hh"
#include "mesh.hh"
#include "mesh_loader.hh"

using namespace std;

Mesh::Mesh() {
  m_vao_handle = 0;
  memset( m_objects, 0, sizeof(m_objects));
}

Mesh::~Mesh() {

  glDeleteBuffers( 2, m_objects );
  gl_catch_errors( "glDeleteBuffers" );

  if(m_vao_handle) { 
    glDeleteVertexArrays( 1, &m_vao_handle );
    gl_catch_errors( "glDeleteVertexArrays" );
  }

  if( m_vertex_data ) free(m_vertex_data);
}

void Mesh::load_from( const MeshLoader &ml ) {

  m_num_vertices = ml.vertex_count();
  m_num_indices  = ml.index_count();

  m_vertex_data = (MeshLoader::node_p)malloc( ml.data_byte_size() );
  if( !m_num_vertices ) raise( "failed to allocate mesh vertex buffer" );

  ml.write_vertex_data_to_buffer( m_vertex_data );

  glGenBuffers(2, m_objects);
  gl_catch_errors( "glGenBuffers" );

  cout << "m_num_vertices=" << m_num_vertices << endl;
  cout << "m_num_indices=" << m_num_indices << endl;

  //
  // create VAO
  //
  glGenVertexArrays(1, &m_vao_handle);
  gl_catch_errors( "glGenVertexArrays" );

  glBindVertexArray(m_vao_handle);
  gl_catch_errors( "glBindVertexArray" );

  glBindBuffer(GL_ARRAY_BUFFER, m_objects[0]);
  gl_catch_errors( "glBindBuffer" );

  //
  // the data...
  //
  glBufferData(GL_ARRAY_BUFFER, ml.data_byte_size(), m_vertex_data, GL_STATIC_DRAW);
  gl_catch_errors( "glBufferData" );

  //
  // define vertex data
  //
  glEnableVertexAttribArray(0);
  gl_catch_errors( "glEnableVertexAttribArray" );

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshLoader::node_t), (void*)NULL);
  gl_catch_errors( "glVertexAttribPointer" );

  //
  // define normal data
  //
  glEnableVertexAttribArray(1);
  gl_catch_errors( "glEnableVertexAttribArray" );

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshLoader::node_t), (void*)(sizeof(float) * 3));
  gl_catch_errors( "glVertexAttribPointer" );

  //
  // create index VBO
  //
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objects[1]);
  gl_catch_errors( "glBindBuffer" );

  /* Upload index data to the video device */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ml.index_byte_size(), ml.index_ptr(), GL_STATIC_DRAW);
  gl_catch_errors( "glBufferData" );

  //
  // done
  //
  glBindVertexArray(0);
  gl_catch_errors( "glEnableVertexAttribArray" );
}

void Mesh::set_shader_attribute( GLuint sa ) {

  m_shader_attribute = sa; 

  /* Specify that our coordinate data is going into attribute index 0(shaderAttribute), and contains three floats per vertex */
  glVertexAttribPointer(sa, 3, GL_FLOAT, GL_FALSE, 0, 0);
  gl_catch_errors( "glVertexAttribPointer" );

  /* Enable attribute index 0(shaderAttribute) as being used */
  glEnableVertexAttribArray(sa);
  gl_catch_errors( "glEnableVertexAttribArray" );
}

GLuint Mesh::object_handle() {
  return m_vao_handle;
}

GLuint Mesh::vertex_object_handle() {
  return m_objects[0];
}

GLuint Mesh::index_object_handle() {
  return m_objects[1];
}

void Mesh::bind() {
  glBindVertexArray (m_vao_handle);
}

void Mesh::draw() {
  //glDrawElements(GL_TRIANGLES, 0, m_num_indices);
}

void Mesh::bind_and_draw() {

  glBindVertexArray (m_vao_handle);

  glDrawElements( GL_TRIANGLES, m_num_indices * 3, GL_UNSIGNED_SHORT, NULL );

  glBindVertexArray(0);
}

