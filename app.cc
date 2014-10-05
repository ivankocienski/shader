
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "app.hh"
#include "globals.hh"
#include "mesh_loader.hh"
#include "mesh.hh"
#include "shader.hh"


const char *vertex_shader = 
  "#version 130\n" // Specify which version of GLSL we are using.
 
  // in_Position was bound to attribute index 0("shaderAttribute")
  "in  vec3 in_Position;\n"
  "in  vec3 in_Normal;\n"
  "uniform vec3 lightDir;\n"
  "varying float intensity;\n"

  "void main()\n"
  "{\n"
  "  intensity = dot(lightDir, in_Normal);\n"
  "  gl_Position = ftransform();\n"
  "}";

const char *fragment_shader = 
  "#version 130\n" // Specify which version of GLSL we are using.
 
  "precision highp float;\n" // Video card drivers require this line to function properly
 
  "out vec4 fragColor;\n"
  "varying float intensity;\n"
 
  "void main()\n"
  "{\n"
  "  fragColor = vec4(intensity, 0.4, 0.0, 1.0);\n" 
  "}";

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

  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );
  //glCullFace( GL_FRONT );

  MeshLoader ml;
  ml.load_from_obj_file( "data/bunny-normalized.obj" );
  //ml.load_from_obj_file( "data/quad.obj" );

  m_mesh.load_from( ml ); 
  m_mesh.set_shader_attribute(0);

  m_shader.load_vertex_from( vertex_shader );
  m_shader.load_fragment_from( fragment_shader );
  
  m_shader.bind_attribute( 0, "in_Position" );
  m_shader.bind_attribute( 1, "in_Normal" );

  m_shader.link(); 

  m_shader.use();
  m_light_vector = m_shader.get_uniform_var( "lightDir" );

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

  glTranslatef( 0, -1.5, 0.0 );
  glScalef( 0.2, 0.2, 0.2 );
  glRotatef( m_angle, 0.0, 0.1, 0 );

  m_shader.use();

  m_light_vector.set( 1, 0, 0 );
  m_mesh.bind_and_draw();

/*   Shader::use_default();
 * 
 *   glColor3f( 0, 1, 0 );
 *   
 *   glBegin(GL_TRIANGLES);
 * 		glVertex3f( -0.5, -0.5, 1.0 );
 * 		glVertex3f(  0.5,  0.0, 1.0 );
 * 		glVertex3f(  0.0,  0.5, 1.0 );
 * 	glEnd();
 */
}

