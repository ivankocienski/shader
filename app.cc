
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "app.hh"
#include "globals.hh"

App::App() { 
}

void App::init() {
  
  m_angle = 0;

  glViewport( 0, 0, g_xres, g_yres ); 

  glMatrixMode(GL_PROJECTION);  
  glLoadIdentity();     
  gluPerspective(40.0, (double)g_xres / (double)g_yres, 0.25, 20.0);
  glMatrixMode(GL_MODELVIEW);

  glClearColor( 0, 0, 1, 1 );

  m_bg.init();

  m_bunny.init();

  m_font.initialize( 25, "data/OCRA.ttf" );

}

void App::tick() {

  m_angle += 0.05;
  
  glLoadIdentity();     

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  gluLookAt(
    /* position */
    0.0, 
    0.0, 
    -5.0, 

    /* center */
    0.0, 
    0.0, 
    0.0, 

    /* UP */
    0.0, 
    1.0, 
    -5.0
  );

  m_bg.draw();

  m_bunny.draw( m_angle );

  m_font.activate();

  m_font.color( 1, 1, 1 );
  m_font.draw( 20, 20, "Stanford Bunny" );

}

