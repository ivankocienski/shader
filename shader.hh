
#pragma once

#include <GL/gl.h>

class Shader {
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

};

