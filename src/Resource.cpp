#include "Resource.h"

Resource::Resource()
{
  if (m_pix_tex.create(1, 1)) {
    //error
  }
  sf::Uint8 pixel[4] = {0xff,0xff,0xff,0xff}; //rgba
  m_pix_tex.update(pixel);
}

Resource::~Resource()
{
}
