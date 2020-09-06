#include "stdafx.h"

#include "Resource.h"

Resource::Resource()
{
  constexpr uint32_t x = 8;
  constexpr uint32_t y = 8;
  if (m_pix_tex.create(x, y)) {
    //error
  }
  sf::Uint8 pixel[4 * x * y];
  std::memset(pixel, 0xff, fw::array_size(pixel));
  m_pix_tex.update(pixel);
}

Resource::~Resource()
{
}

void Resource::setup_at_boot()
{
  sf::Image img;
  if (img.loadFromFile("rom/sprites.gif")) {
    auto msk_col = img.getPixel(0,0);
    img.createMaskFromColor(msk_col);
    if (!m_spr_tex.loadFromImage(img)) FW_ASSERT(0);
  } else {
    FW_ASSERT(0);
  }
}

sf::IntRect Resource::get_spr_rect(uint32_t id)
{
  int row = id % 16;
  int col = id / 16;
  return sf::IntRect(row*8, col*8, 8, 8);
}
