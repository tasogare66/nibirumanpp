#include "stdafx.h"

#include <fstream>

#include "Resource.h"

Resource::Resource()
{
  constexpr uint32_t x = 1;
  constexpr uint32_t y = 1;
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

inline std::vector<uint8_t> read_vector_from_file(std::string_view file_path)
{
  std::ifstream instream(file_path.data(), std::ios::in | std::ios::binary);
  std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
  return data;
}

void Resource::setup_at_boot()
{
  //sprite
  sf::Image img;
  if (img.loadFromFile("rom/sprites.gif")) {
    auto msk_col = img.getPixel(0,0);
    img.createMaskFromColor(msk_col);
    if (!m_spr_tex.loadFromImage(img)) FW_ASSERT(0);
    m_spr_tex.setSmooth(false);
  } else {
    FW_ASSERT(0);
  }
  //font
  if (!m_base_font.loadFromFile("rom/heavy_data.ttf")) FW_ASSERT(0);
  //map
  m_map = read_vector_from_file("rom/world.map");
  FW_ASSERT(m_map.size()>0);
}

sf::IntRect Resource::get_spr_rect(uint32_t id)
{
  int row = id % 16;
  int col = id / 16;
  return sf::IntRect(row*8, col*8, 8, 8);
}
