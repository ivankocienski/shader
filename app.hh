
#pragma once

#include "font.hh"

#include "bunny.hh"
#include "background.hh"

class App {
private:

  Font   m_font;
  float  m_angle; 

  Bunny m_bunny;
  Background m_bg;

  

public:

  App();

  void init();
  void tick();
};

