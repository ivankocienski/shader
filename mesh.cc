
#include <GL/glew.h>
#include <GL/gl.h>

#include "gl_error.hh"
#include "mesh.hh"
#include "mesh_loader.hh"

Mesh::Mesh() {
  m_vao_handle = 0;
  m_vbo_handle = 0;
}

Mesh::~Mesh() {
  if(m_vbo_handle) {
    glDeleteBuffers( 1, &m_vbo_handle );
    gl_catch_errors( "glDeleteBuffers" );
  }

  if(m_vao_handle) { 
    glDeleteVertexArrays( 1, &m_vao_handle );
    gl_catch_errors( "glDeleteVertexArrays" );
  }
}

bool Mesh::load_from( const MeshLoader &ml ) {

  m_num_vertices = ml.vertex_count();

  //VBO
  glGenBuffers(1, &m_vbo_handle);
  gl_catch_errors( "glGenBuffers" );

  /* Make the new VBO active */
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handle);
  gl_catch_errors( "glBindBuffer" );

  /* Upload vertex data to the video device */
  glBufferData(GL_ARRAY_BUFFER, ml.vertex_byte_size(), ml.vertex_ptr(), GL_STATIC_DRAW);
  gl_catch_errors( "glBufferData" );



  //VAO
  glGenVertexArrays (1, &m_vao_handle);
  gl_catch_errors( "glGenVertexArrays" );

  glBindVertexArray (m_vao_handle);
  gl_catch_errors( "glBindVertexArray" );

  glEnableVertexAttribArray (0);
  gl_catch_errors( "glEnableVertexAttribArray" );

  glBindBuffer (GL_ARRAY_BUFFER, m_vbo_handle);
  gl_catch_errors( "glBindBuffer" );

  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  gl_catch_errors( "glVertexAttribPointer" );
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

GLuint Mesh::vao_handle() {
  return m_vao_handle;
}

GLuint Mesh::vbo_handle() {
  return m_vbo_handle;
}

void Mesh::bind() {
  glBindVertexArray (m_vao_handle);
}

void Mesh::draw() {
  glDrawArrays(GL_TRIANGLES, 0, m_num_vertices);
}

void Mesh::bind_and_draw() {
  glBindVertexArray (m_vao_handle);
  glDrawArrays(GL_TRIANGLES, 0, m_num_vertices);
}

