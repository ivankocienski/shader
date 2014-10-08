
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.hh"

/* NOTE: at the moment only ONE font is handled. boo! */

class Font;


class FontRenderer {
private:

  FT_Library m_library;
  FT_Face m_face;

  int m_size;

  bool  m_ch_draw;
  float m_ch_t_xo;
  float m_ch_t_yo;
  float m_ch_w;
  float m_ch_h;
  int   m_ch_x_inc;

public:

  FontRenderer( int, const char* );

  void render( char, GLuint );

  bool  draw();
  float t_xo();
  float t_yo();
  float w();
  float h();
  int   x_inc();
};

class Font {
public:

  typedef struct {
    bool draw;
    float t_xo;
    float t_yo;
    float w;
    float h;
    int x_inc;
  } char_t;

private:

  Shader m_font_shader;

  GLuint m_vbo;
  GLuint m_textures[128];

  ShaderUniformVar m_color;
  
  char_t m_chars[128];
  //int m_width[128];
  //int m_height;

  void setup_shader();
  void setup_vbo();
  void setup_font( int, const char* );

public:
  
  Font();

  void initialize( int, const char* );
  void draw( int, int, const char* );
  void color( float, float, float );

  void hack();
};

