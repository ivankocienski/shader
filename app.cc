
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "app.hh"
#include "globals.hh"
#include "mesh_loader.hh"
#include "mesh.hh"
#include "shader.hh"

App::App() { 
}

void App::init() {
  
  m_angle = 0;

  glViewport( 0, 0, g_xres, g_yres ); 

  glMatrixMode(GL_PROJECTION);  
  glLoadIdentity();     
  gluPerspective(40.0, (double)g_xres / (double)g_yres, 0.25, 20.0);
  glMatrixMode(GL_MODELVIEW);

  glClearColor( 0, 0, 1, 0 );

  //glEnable( GL_DEPTH_TEST );
  //glEnable( GL_CULL_FACE );

  //MeshLoader ml;
  //ml.load_from_obj_file( "data/bunny-normalized.obj" );
  //ml.load_from_obj_file( "data/quad.obj" );

  //m_mesh.load_from( ml ); 
  //m_mesh.set_shader_attribute(0);

  //m_shader.vertex().load_from( "data/vertex.vs" );
  //m_shader.fragment().load_from( "data/fragment.fs" );
  
  //m_shader.bind_attribute( 0, "in_Position" );
  //m_shader.bind_attribute( 1, "in_Normal" );

  //m_shader.link(); 

  //m_shader.use();
  //m_light_vector = m_shader.get_uniform_var( "lightDir" );

  m_font.initialize( 25, "data/OCRA.ttf" );

}

void App::tick() {

  m_angle += 0.05;
  
  glLoadIdentity();     

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


  m_font.color( 0, 0, 0 );
  m_font.draw( 21, 21, "Stanford Bunny" );

  m_font.color( 1, 1, 1 );
  m_font.draw( 20, 20, "Stanford Bunny" );


  //m_font.hack();

  return;

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


  glPushMatrix();

  glTranslatef( 0, -1.5, 0.0 );
  glScalef( 0.2, 0.2, 0.2 );
  glRotatef( m_angle, 0.0, 0.1, 0 );

  m_shader.use();

  m_light_vector.set( 1, 0, 0 );
  m_mesh.bind_and_draw();

  glPopMatrix();

}

