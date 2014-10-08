
#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <cstdlib>
#include <cstring>

#include "shader.hh"
#include "gl_error.hh"

/*
 * SHADER UNIFORM VARIABLE CLASS
 *
 * Wraps the getting and setting of variables CPU -> GPU
 *
 */

using namespace std;

ShaderUniformVar::ShaderUniformVar() { 
  m_handle = 0;
}

ShaderUniformVar::ShaderUniformVar( GLuint p_id, const char *n ) {
  m_handle = glGetUniformLocation(p_id, n);
  gl_catch_errors( "glUseProgram" );
}

void ShaderUniformVar::set( float v ) {
  glUniform1f(m_handle, v);
  gl_catch_errors( "glUniform1f" );
}

void ShaderUniformVar::set( float v1, float v2 ) {
  float a[] = { v1, v2 };
  glUniform2fv(m_handle, 1, a);
  gl_catch_errors( "glUniform2fv" );
}

void ShaderUniformVar::set( float v1, float v2, float v3 ) {
  float a[] = { v1, v2, v3 };
  glUniform3fv(m_handle, 1, a);
  gl_catch_errors( "glUniform3fv" );
}

void ShaderUniformVar::set( float v1, float v2, float v3, float v4 ) {
  float a[] = { v1, v2, v3, v4 };
  glUniform4fv(m_handle, 1, a);
  gl_catch_errors( "glUniform4fv" );
}

/* 
 * SHADER COMPONENT
 *
 * Wraps loading text files into shader contexts, cleaning up, etc
 *
 */

ShaderComponent::ShaderComponent( GLuint t ) {
  m_handle = 0;
  m_type   = t;
}

ShaderComponent::~ShaderComponent() {
  if( m_handle ) glDeleteShader( m_handle );
}

void ShaderComponent::load_from( const char* path ) {

  GLint compiled;
  int   out_buffer_len;
  char* out_buffer;
  int   out_buffer_written;

  vector<char*> code_strings;

  {
    ifstream file( path );
    string line;

    /* load the file into the vectors. is this overkill? yes.
     * todo: make this better at memory managenemnt (use one
     * string buffer and load everything into it) */
    while( getline( file, line ) ) {

      line += "\n";

      int len = line.length() + 1;

      char *s = (char*)malloc( len * sizeof(char) );
      if( !s ) {
        kraise( "failed to malloc string space" ); 
      }

      memcpy( s, line.c_str(), len );
      code_strings.push_back( s ); 
    }
  }

  m_handle = glCreateShader(m_type);
  gl_catch_errors( "glCreateShader" );

  /* Associate the source code buffers with each handle */
  glShaderSource( m_handle, code_strings.size(), (const GLchar **)code_strings.data(), NULL);
  gl_catch_errors( "glShaderSource" );

  /* Compile our shader objects */
  glCompileShader( m_handle );
  gl_catch_errors( "glCompileShader" );

  glGetShaderiv( m_handle, GL_COMPILE_STATUS, &compiled);
  gl_catch_errors( "glGetShaderiv" );

  if(!compiled) {

    std::cerr << "failed to compile " << path << std::endl;
  }

  glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &out_buffer_len);
  gl_catch_errors( "glGetShaderiv" );

  if (out_buffer_len > 1)
  {
    out_buffer = new char [out_buffer_len];
    if(!out_buffer ) 
      kraise( "could not alloc out_buffer when compiling shader" );

    glGetShaderInfoLog(m_handle, out_buffer_len, &out_buffer_written, out_buffer);
    gl_catch_errors( "glGetShaderInfoLog" );

    out_buffer_len--; //terminating \0

    if( out_buffer_written != out_buffer_len ) {
      std::cerr << "W: out_buffer_written (" << out_buffer_written << ") != out_buffer_len (" << out_buffer_len << ")" << std::endl;
    }

    std::cerr << out_buffer << std::endl; 
  }

  {
    vector<char*>::iterator it;

    for( it = code_strings.begin(); it != code_strings.end(); it++ ) {
      free( *it );
    }

  }

  if( !compiled ) kraise( "failed to compile shader" );
}

void ShaderComponent::attach_to( GLuint prog_id ) {
  glAttachShader(prog_id, m_handle);
  gl_catch_errors( "glAttachShader" );
}


/*
 * SHADER CLASS
 *
 * Actual shader program interface
 *
 */


Shader::Shader() : m_vertex( GL_VERTEX_SHADER ), m_fragment( GL_FRAGMENT_SHADER ) {
  m_program = 0;
}

Shader::~Shader() {
  if( m_program ) glDeleteProgram( m_program );
}

ShaderComponent & Shader::vertex() {

  create_program();

  return m_vertex;
}

ShaderComponent & Shader::fragment() {

  create_program();

  return m_fragment;
}

void Shader::bind_attribute( GLuint a, const char *n ) {

  create_program();

  glBindAttribLocation(m_program, a, n );
  gl_catch_errors( "glBindAttribLocation" );
}

void Shader::link() {

  GLint okay;
  GLint log_len;

  create_program();

  m_vertex.attach_to( m_program );
  m_fragment.attach_to( m_program );

  glLinkProgram(m_program);
  gl_catch_errors( "glLinkProgram" );

  glGetProgramiv( m_program, GL_LINK_STATUS, &okay );

  if( !okay ) {
    cerr << "failed to link" << endl;
  }

  {
    GLchar *log_buffer = new GLchar [8192];
    if( !log_buffer ) kraise("could not alloc log buffer");

    glGetProgramInfoLog( m_program, 8192, &log_len, log_buffer );
    gl_catch_errors( "glGetProgramInfoLog" );

    if( log_len > 0 )
      cerr << log_buffer << endl;

    delete log_buffer;
  }

  if( !okay ) kraise( "stopping" );
}

void Shader::create_program() {
  if( m_program) return;

  m_program = glCreateProgram();
  gl_catch_errors( "glCreateProgram" ); 
}

void Shader::use() {
  glUseProgram(m_program);
  gl_catch_errors( "glUseProgram" );
}

ShaderUniformVar Shader::get_uniform_var( const char *n ) {
  return ShaderUniformVar( m_program, n );
}

GLint Shader::get_attribute_var( const char *n ) {
  return glGetAttribLocation( m_program, n );
}
