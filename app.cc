
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "app.hh"
#include "globals.hh"

static float vertex_data[3][3] = {
  {  0.0,  1.0, 1.0  },
  { -1.0, -1.0, 1.0  },
  {  1.0, -1.0, 1.0  }
};

const char *vertex_shader = 
  "#version 130\n" // Specify which version of GLSL we are using.
 
  // in_Position was bound to attribute index 0("shaderAttribute")
  "in  vec3 in_Position;\n"

  "void main()\n"
  "{\n"
  "  gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);\n"
  "}";

const char *fragment_shader = 
  "#version 130\n" // Specify which version of GLSL we are using.
 
  "precision highp float;\n" // Video card drivers require this line to function properly
 
  "out vec4 fragColor;\n"
 
  "void main()\n"
  "{\n"
  "  fragColor = vec4(1.0,0.0,0.0,1.0);\n" 
  "}";

App::App() { 
}

void App::init() {
  
  glViewport( 0, 0, g_xres, g_yres ); 

  glMatrixMode(GL_PROJECTION);  
  glLoadIdentity();     
  gluPerspective(40.0, (double)g_xres / (double)g_yres, 0.25, 20.0);
  glMatrixMode(GL_MODELVIEW);

  glClearColor( 0, 0, 1, 0 );

  m_mesh.load_from( 3, (float *)vertex_data ); 
  m_mesh.set_shader_attribute(0);

  m_shader.load_vertex_from( vertex_shader );
  m_shader.load_fragment_from( fragment_shader );
  
  m_shader.bind_attribute( 0, "in_Position" );

  m_shader.link(); 
}

void App::tick() {

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

  m_shader.use();
  m_mesh.bind_and_draw();

  Shader::use_default();

  glColor3f( 0, 1, 0 );
  
  glBegin(GL_TRIANGLES);
		glVertex3f( -0.5, -0.5, 1.0 );
		glVertex3f(  0.5,  0.0, 1.0 );
		glVertex3f(  0.0,  0.5, 1.0 );
	glEnd();
}

