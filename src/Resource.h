#pragma once
#include "Singleton.h"

class Resource : public Singleton<Resource> {
public:
  Resource();
  ~Resource();
  void setup_at_boot();
  const sf::Texture& get_pix_tex() const { return m_pix_tex; }
  const sf::Texture& get_spr_tex() const { return m_spr_tex; }
  static sf::IntRect get_spr_rect(uint32_t id);
  const sf::Font& get_base_font() const { return m_base_font; }
  const std::vector<uint8_t>& get_map() const { return m_map; }
private:
  sf::Texture m_pix_tex;
  sf::Texture m_spr_tex;
  sf::Font m_base_font;
  std::vector<uint8_t> m_map;
};