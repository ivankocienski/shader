
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "background.hh"
#include "png.hh"

/*  ever remember when backgrounds were just a RAM buffer loaded from disk
 *  and memcpy'd to the framebuffer? */

Background::Background() { }

  GLfloat box[4][4] = {
    { -1, -1, 0, 0},
    {  1, -1, 1, 0},
    { -1,  1, 0, 1},
    {  1,  1, 1, 1},
  };

void Background::init_shader() {

  m_shader.vertex().load_from(   "data/background.vs" );
  m_shader.fragment().load_from( "data/background.fs" );
  m_shader.link();
  m_shader.use();
}

void Background::init_texture() {

  Png png;

  png.load( "data/background.png" );

  glGenTextures( 1, &m_texture );
  glBindTexture( GL_TEXTURE_2D, m_texture );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  glTexImage2D(
    GL_TEXTURE_2D, 
    0, 
    GL_RGB, 
    png.width(),
    png.height(),
    0,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    png.bytes()
  );
}

void Background::init_vbo() {

  glGenVertexArrays( 1, &m_vao );
  gl_catch_errors( "glGenVertexArrays" );

  glBindVertexArray( m_vao );
  gl_catch_errors( "glBindVertexArray" );


  glGenBuffers(1, &m_vbo);
  gl_catch_errors( "glGenBuffers" );

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  gl_catch_errors( "glBindBuffer" );


  glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
  gl_catch_errors( "glBufferData" );


  GLint a = m_shader.get_attribute_var( "coord" );
  gl_catch_errors( "get_attribute_var" );

  glEnableVertexAttribArray(a);
  gl_catch_errors( "glEnableVertexAttribArray" );

  glVertexAttribPointer(a, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
  gl_catch_errors( "glVertexAttribPointer" ); 

  glBindVertexArray( 0 ); 
}

void Background::init() {

  init_shader();

  init_texture();

  init_vbo(); 
}

void Background::draw() {
  
  glEnable( GL_TEXTURE_2D );

  glBindTexture( GL_TEXTURE_2D, m_texture );
  gl_catch_errors( "glBindTexture" ); 

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_CULL_FACE );

  m_shader.use(); 

  glBindVertexArray(m_vao );
  gl_catch_errors( "glBindVertexArray" );

  //glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  //gl_catch_errors( "glBindBuffer" ); 
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
  gl_catch_errors( "glDrawArrays" ); 

  glBindVertexArray(0);
}

