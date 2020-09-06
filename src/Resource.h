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
private:
  sf::Texture m_pix_tex;
  sf::Texture m_spr_tex;
};