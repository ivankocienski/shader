
#pragma once

#include <boost/noncopyable.hpp>

#include <GL/gl.h>
#include "gl_error.hh"

class ShaderUniformVar  {
private:

  GLuint m_handle;

public:

  ShaderUniformVar();
  ShaderUniformVar(GLuint, const char* );

  void set( float );
  void set( float, float, float );
};

class ShaderComponent : boost::noncopyable {
private:

  GLuint m_handle;
  GLuint m_type;

public:

  ShaderComponent( GLuint );
  ~ShaderComponent();

  void load_from( const char* );
  void attach_to( GLuint );

};

class Shader {
public:

  static void use_default() {
    glUseProgram(0);
    gl_catch_errors( "glUseProgram" );
  }

private:

  GLuint m_program;

  ShaderComponent m_vertex;
  ShaderComponent m_fragment;

  void create_program();

public:

  Shader();
  ~Shader();

  ShaderComponent &vertex();
  ShaderComponent &fragment();

  void bind_attribute( GLuint, const char* );
  void link();
  void use();

  ShaderUniformVar get_uniform_var( const char* );

};

