
#include <iostream>

#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "png.hh"

using namespace std;

static const int sig_len = 8;

static void png_error_fnc(png_structp png_ptr, png_const_charp error_msg) {
  cerr << "E: png: " << error_msg << endl;
  longjmp( png_jmpbuf(png_ptr), 1 );
}

static void png_warning_fnc(png_structp png_ptr, png_const_charp warning_msg) {
  printf( "W: png: %s\n", warning_msg ); 
}

Png::Png() {
  m_width  = 0;
  m_height = 0;
}

int Png::width() {
  return m_width;
}

int Png::height() {
  return m_height;
}

unsigned char* Png::bytes() {
  return m_bytes.data();
}

bool Png::load( const char *fn ) {

  bool okay = false;
  FILE *file = NULL;
  unsigned char *data = NULL;
  png_structp png_ptr = NULL;
  png_infop info_ptr  = NULL;

  unsigned char sig[8];

  int i_bits;
  uint32_t i_width;
  uint32_t i_height;
  int i_depth;
  int i_ctype;
  int i_passes;

  unsigned char *i_data = NULL;
  png_uint_32  rowbytes;
  png_bytepp  row_pointers = NULL;
  png_bytepp rp;
  unsigned char *ip;
  int i;

  for( ;; ) {

    /*  file stuff */
    
    file = fopen( fn, "r" );
    if(!file) break;

    printf( "reading %s\n", fn );

    fread(sig, sizeof( unsigned char ), sig_len, file);
    if (!png_check_sig(sig, sig_len)) break;

    /* inialize control structures */
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, png_error_fnc, png_warning_fnc);
    if (!png_ptr) break;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) break;

    if (setjmp(png_jmpbuf(png_ptr))) break;

    /* read the header */

    png_init_io(png_ptr, file);

    png_set_sig_bytes(png_ptr, sig_len);  /* we already read the 8 signature bytes */

    png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */

    png_get_IHDR(png_ptr, info_ptr, &i_width, &i_height, &i_depth, &i_ctype, NULL, NULL, NULL); 
    printf("w=%d  h=%d  bits=%d   color_type=%d\n", i_width, i_height, i_depth, i_ctype);

    i_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);
    printf("passes=%d\n", i_passes);

    /* now read the data */

    rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    printf( "rowbytes=%d\n", rowbytes );

    m_bytes.resize( rowbytes * i_height );

    row_pointers = (png_bytepp)malloc(i_height * sizeof(png_bytep));
    if( !row_pointers ) break;

    rp = row_pointers;
    ip = m_bytes.data();
    for( i = 0; i < i_height; i++ ) {
      *rp = ip;
      rp++;
      ip += rowbytes;
    }

    png_read_image(png_ptr, row_pointers);

    
    m_width  = i_width;
    m_height = i_height;

    // done !

    okay = true;

    break;
  }

  if( row_pointers ) free( row_pointers );

  png_destroy_read_struct( &png_ptr, &info_ptr, NULL );

  if( file ) fclose( file );

  return okay;
}

