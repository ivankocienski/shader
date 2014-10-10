
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "bunny.hh"
#include "mesh_loader.hh"
#include "mesh.hh"
#include "shader.hh"

Bunny::Bunny() {
}

void Bunny::init() {

  MeshLoader ml;
  ml.load_from_obj_file( "data/bunny-normalized.obj" );


  m_shadow_shader.vertex().load_from( "data/bunny-shadow.vs" );
  m_shadow_shader.fragment().load_from( "data/bunny-shadow.fs" );

  m_shadow_shader.bind_attribute( 0, "in_Position" );
  m_shadow_shader.bind_attribute( 1, "in_Normal" );

  m_shadow_shader.link(); 

  m_mesh.load_from( ml ); 

  m_shader.vertex().load_from( "data/bunny.vs" );
  m_shader.fragment().load_from( "data/bunny.fs" );
  
  m_shader.bind_attribute( 0, "in_Position" );
  m_shader.bind_attribute( 1, "in_Normal" );

  m_shader.link(); 

  m_shader.use();
  m_light_vector = m_shader.get_uniform_var( "lightDir" );

  m_mesh.set_shader_attribute(0);

}

void Bunny::draw( float a ) {

  glEnable( GL_CULL_FACE );
  m_mesh.bind();

  /* shadow */
  glPushMatrix(); 

    glTranslatef( -0.5, -0.9, 0.0 );
    glScalef( 0.2, 0.2, 0.2 );
    glRotatef( a, 0.0, 0.1, 0 );
    
    m_shadow_shader.use();
    m_mesh.draw();

  glPopMatrix();

  /* bunny */
  glPushMatrix();

    glEnable( GL_DEPTH_TEST );

    glTranslatef( 0, -0.7, 0.0 );
    glScalef( 0.2, 0.2, 0.2 );
    glRotatef( a, 0.0, 0.1, 0 );

    m_shader.use();

    m_light_vector.set( -1, 0, 0 );
    m_mesh.draw();

    glDisable( GL_DEPTH_TEST );

  glPopMatrix();

  glDisable( GL_CULL_FACE );

  glBindVertexArray(0);
}

