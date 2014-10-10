
#pragma once

#include <vector>

class Png {

private:

  int m_width;
  int m_height;
  std::vector<unsigned char> m_bytes;

public:

  Png();

  bool load( const char * );

  int width();
  int height();
  unsigned char *bytes();
};

