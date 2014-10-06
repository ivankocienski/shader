
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.hh"

/* NOTE: at the moment only ONE font is handled. boo! */

class Font {
private:
 
  FT_Library m_library;
  FT_Face m_face;
  
  Shader m_font_shader;

  GLuint m_textures[128];
  int m_width[128];
  int m_height;

public:
  
  Font();

  void initialize( int, const char* );
  void draw( int, int, const char* );
};

