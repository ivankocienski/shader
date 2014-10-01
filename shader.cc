
#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

#include "shader.hh"
#include "gl_error.hh"

Shader::Shader() {
  m_vertex   = 0;
  m_fragment = 0;
  m_program  = 0;
}

Shader::~Shader() {
  //TODO: cleanup
}

void Shader::load_vertex_from( const char* src ) {

  create_program();

  m_vertex = glCreateShader(GL_VERTEX_SHADER);
  gl_catch_errors( "glCreateShader" );

  do_compile( m_vertex, src, "vertex" );

  glAttachShader(m_program, m_vertex);
  gl_catch_errors( "glAttachShader" );
}

void Shader::load_fragment_from( const char* src ) {

  create_program();

  m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
  gl_catch_errors( "glCreateShader" );

  do_compile( m_fragment, src, "fragment" );

  glAttachShader(m_program, m_fragment);
  gl_catch_errors( "glAttachShader" );

}

void Shader::bind_attribute( GLuint a, const char *n ) {

  glBindAttribLocation(m_program, a, n );
  gl_catch_errors( "glBindAttribLocation" );
}

void Shader::link() {

  glLinkProgram(m_program);
  gl_catch_errors( "glLinkProgram" );
}

void Shader::create_program() {

  if( m_program ) return; 

  m_program = glCreateProgram();
  gl_catch_errors( "glCreateProgram" ); 
}

void Shader::do_compile( GLuint target, const char* src, const char* type ) {

  GLint compiled;
  int   out_buffer_len;
  char* out_buffer;
  int   out_buffer_written;

  /* Associate the source code buffers with each handle */
  glShaderSource(target, 1, (const GLchar**)&src, 0);
  gl_catch_errors( "glShaderSource" );

  /* Compile our shader objects */
  glCompileShader(target);
  gl_catch_errors( "glCompileShader" );

  glGetShaderiv(target, GL_COMPILE_STATUS, &compiled);
  gl_catch_errors( "glGetShaderiv" );

  if(!compiled) {

    std::cerr << "failed to compile " << type << " shader" << std::endl;

    glGetShaderiv(target, GL_INFO_LOG_LENGTH, &out_buffer_len);
    gl_catch_errors( "glGetShaderiv" );

    if (out_buffer_len)
    {
      out_buffer = new char [out_buffer_len];
      if(!out_buffer ) 
        raise( "could not alloc out_buffer when compiling shader" );

      glGetShaderInfoLog(target, out_buffer_len, &out_buffer_written, out_buffer);
      gl_catch_errors( "glGetShaderInfoLog" );

      if( out_buffer_written != out_buffer_len ) {
        std::cerr << "W: out_buffer_written != out_buffer_len" << std::endl;
      }

      std::cerr << out_buffer << std::endl;

      raise( "failed to compile shader" );
    }
  }
}

void Shader::use() {
  glUseProgram(m_program);
  gl_catch_errors( "glUseProgram" );
}
