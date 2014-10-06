
#include <GL/glew.h>
#include <GL/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "font.hh"
#include "gl_error.hh"
#include "shader.hh"


Font::Font() {
  memset( m_textures, 0, sizeof(m_textures));
  m_library = NULL; 
}

void Font::initialize( int size, const char *path ) {

  int err;
  int i;

  m_height = size;

  /* FT setup */
  err = FT_Init_FreeType( &m_library );
  if ( err ) kraise( "FT_Init_FreeType" );

  err = FT_New_Face( m_library, path, 0, &m_face );
  if( err ) kraise( "FT_New_Face" );

  err = FT_Set_Pixel_Sizes( m_face, 0, size );
  if( err ) kraise( "FT_Set_Pixel_Sizes" );

  /* GL setup */

  m_font_shader.vertex().load_from( "data/font-shader.vs" );
  m_font_shader.fragment().load_from( "data/font-shader.fs" );
  m_font_shader.link();

  glGenTextures( 128, m_textures );
  gl_catch_errors( "glGenTextures" );

  kraise( "ooops" );

  for( i = 0; i < 128; i++ ) {

    /*** Set-up / render glyph ***/

    FT_UInt glyph_index = FT_Get_Char_Index( m_face, i );

    err = FT_Load_Glyph( m_face, glyph_index, FT_LOAD_DEFAULT );
    if( err ) kraise( "FT_Load_Glyph" );

    err = FT_Render_Glyph( m_face->glyph, FT_RENDER_MODE_NORMAL );
    if( err ) kraise( "FT_Render_Glyph" );

    m_width[i] = m_face->glyph->bitmap.width;

    /* 
     * setup GL texture
     *
     */

    glBindTexture(GL_TEXTURE_2D, m_textures[i]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* upload */
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


}

void Font::draw( int x, int y, const char *text ) {

  m_font_shader.use();

  while( *text ) {

    int w = m_width[(unsigned char)*text];

    GLfloat box[4][4] = {
        {(float)x,            (float)y    , 0, 0},
        {(float)x + (float)w, (float)y    , 1, 0},
        {(float)x,            (float)y - (float)m_height, 0, 1},
        {(float)x + (float)w, (float)y - (float)m_height, 1, 1},
    };
 
    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    x += w;
    text++; 
  }
}

