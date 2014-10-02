
#pragma once

#include <GL/gl.h>
#include <sstream>
#include <string>

#include "exception.hh"

#define gl_catch_errors(msg) { \
  int err = glGetError(); \
  if( err != GL_NO_ERROR ) { \
    std::stringstream ss; \
    ss << "GL error (" << err << ") " << gluErrorString(err) << ": " << msg; \
    std::string str(ss.str()); \
    raise(str); \
  } \
}

