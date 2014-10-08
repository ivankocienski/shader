
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "font.hh"
#include "gl_error.hh"
#include "shader.hh"
#include "globals.hh"

using namespace std;

/* font renderer */

FontRenderer::FontRenderer( int size, const char *p ) {

  m_size = size;

	if(FT_Init_FreeType( &m_library )) kraise( "FT_Init_FreeType failed" );

	if(FT_New_Face(m_library, p, 0, &m_face)) kraise( "FT_New_Face failed" );

	FT_Set_Pixel_Sizes(m_face, 0, size);
}

void FontRenderer::render( char ch, GLuint tx ) {

	glBindTexture(GL_TEXTURE_2D, tx);
  gl_catch_errors( "glBindTexture" );

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  gl_catch_errors( "glPixelStorei" );

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  FT_UInt glyph_index = FT_Get_Char_Index( m_face, ch );

  FT_Load_Glyph( m_face, glyph_index, FT_LOAD_DEFAULT );

  FT_Render_Glyph( m_face->glyph, FT_RENDER_MODE_NORMAL );

  m_ch_draw  = false;
  m_ch_x_inc = m_face->glyph->advance.x >> 6;

  if( ch <= 32 ) return;

  m_ch_draw = true;
  m_ch_w    = m_face->glyph->bitmap.width;
  m_ch_h    = m_face->glyph->bitmap.rows;
  m_ch_t_yo = m_size - m_face->glyph->bitmap_top;

  /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
  glTexImage2D(
    GL_TEXTURE_2D, 
    0, 
    GL_ALPHA, 
    m_face->glyph->bitmap.width,
    m_face->glyph->bitmap.rows,
    0,
    GL_ALPHA,
    GL_UNSIGNED_BYTE,
    m_face->glyph->bitmap.buffer
  );
}

bool  FontRenderer::draw()  { return m_ch_draw; }
float FontRenderer::t_yo()  { return m_ch_t_yo; }
float FontRenderer::w()     { return m_ch_w; }
float FontRenderer::h()     { return m_ch_h; }
int   FontRenderer::x_inc() { return m_ch_x_inc; }

/* main font API */

Font::Font() {
  memset( m_textures, 0, sizeof(m_textures));
  memset( m_chars, 0, sizeof(m_chars));
}

void Font::setup_shader() {

  /* GL setup */

  m_font_shader.vertex().load_from( "data/font-shader.vs" );
  m_font_shader.fragment().load_from( "data/font-shader.fs" );
  m_font_shader.link();
  m_font_shader.use();
}

void Font::setup_vbo() {

  glGenBuffers(1, &m_vbo);
  gl_catch_errors( "glGenBuffers" );

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  gl_catch_errors( "glBindBuffer" );


  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, NULL, GL_DYNAMIC_DRAW);
  gl_catch_errors( "glBufferData" );

  ShaderUniformVar m = m_font_shader.get_uniform_var( "screen_res" );
  m.set( g_xres , g_yres  );


  GLint a = m_font_shader.get_attribute_var( "coord" );
  gl_catch_errors( "get_attribute_var" );

  glEnableVertexAttribArray(a);
  gl_catch_errors( "glEnableVertexAttribArray" );

  glVertexAttribPointer(a, 4, GL_FLOAT, GL_FALSE, 0, 0);
  gl_catch_errors( "glVertexAttribPointer" );

  m_color = m_font_shader.get_uniform_var( "color" );
}

void Font::setup_font( int size, const char *path ) {

  glActiveTexture(GL_TEXTURE0);
  gl_catch_errors( "glActiveTexture" );


  glGenTextures( 128, m_textures );
  gl_catch_errors( "thing" );

  FontRenderer fr( size, path ); 
  for( int i = 0; i < 128; i++ ) {
    fr.render( (char)i, m_textures[i] );

    m_chars[i].draw  = fr.draw();
    m_chars[i].t_yo  = fr.t_yo();
    m_chars[i].w     = fr.w();
    m_chars[i].h     = fr.h();
    m_chars[i].x_inc = fr.x_inc();
  }

}

void Font::initialize( int size, const char *path ) {

  setup_shader();

  setup_vbo();

  setup_font( size, path );
}

void Font::color( float r, float g, float b ) { 
  m_color.set( r, g, b, 1 );
}

void Font::activate() { 

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_CULL_FACE );

  glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  m_font_shader.use(); 
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void Font::draw( int x, int y, const char *text ) {

  while( *text ) {

    char_t *ct = &m_chars[*text];

    if( ct->draw ) {

      float px = x; //+ ct->t_xo;
      float py = y + ct->t_yo;

      GLfloat box[4][4] = {
        { px,         py, 0, 0},
        { px + ct->w, py, 1, 0},
        { px,         py + ct->h, 0, 1},
        { px + ct->w, py + ct->h, 1, 1},
      };

      glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(box), box );

      glBindTexture(GL_TEXTURE_2D, m_textures[*text]);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    x += ct->x_inc;
    text++; 
  }
}

