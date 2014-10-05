
#pragma once

#include <sstream>

#define kraise(msg) { \
  std::stringstream ss; \
\
  ss << "("   << __FILE__; \
  ss << " @ " << __LINE__; \
  ss << ") "  << msg; \
\
  throw ss.str().c_str(); \
}
