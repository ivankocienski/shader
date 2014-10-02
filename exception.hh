
#pragma once

#include <sstream>

#define raise(msg) { \
  std::stringstream ss; \
\
  ss << "("   << __FILE__; \
  ss << " @ " << __LINE__; \
  ss << ") "  << msg; \
\
  throw ss.str().c_str(); \
}
