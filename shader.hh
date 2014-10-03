
#pragma once

#include <GL/gl.h>
#include "gl_error.hh"

class ShaderUniformVar {
private:

  GLuint m_handle;

public:

  ShaderUniformVar();
  ShaderUniformVar(GLuint, const char* );

  void set( float );
  void set( float, float, float );
};

class Shader {
public:

  static void use_default() {
    glUseProgram(0);
    gl_catch_errors( "glUseProgram" );
  }

private:

  GLuint m_vertex;
  GLuint m_fragment;
  GLuint m_program;

  void create_program();
  void do_compile( GLuint, const char*, const char* );

public:

  Shader();
  ~Shader();

  void load_vertex_from( const char* );
  void load_fragment_from( const char* );

  void bind_attribute( GLuint, const char* );
  void link();

  void use();

  ShaderUniformVar get_uniform_var( const char* );

};

