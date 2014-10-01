
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "app.hh"
#include "exception.hh"
#include "globals.hh"

using namespace std;

int main( int argc, char** argv ) {

  int code = 0;

  try {

    App app;
    GLFWwindow* window;

    if(!glfwInit()) {
      raise( "could not init glfw" );
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow( g_xres, g_yres, "shader demo", NULL, NULL );
    if(!window) {
      raise( "could not open window" );
    }
    
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK) {
      raise( "could not init GLEW" );
    }

    app.init();

    cout << "OpenGL version : " << glGetString(GL_VERSION) << endl;
    cout << "GLSL version   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "Vendor         : " << glGetString(GL_VENDOR) << endl;
    cout << "Renderer       : " << glGetString(GL_RENDERER) << endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      app.tick();

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    } 

  } catch( char const* msg ) {
    cerr << "E: " << msg << endl; 
    code = -1;
  }

  glfwTerminate();

  return code;
}
