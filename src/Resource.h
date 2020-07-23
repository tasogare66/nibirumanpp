#pragma once
#include <SFML\Graphics.hpp>
#include "Singleton.h"

class Resource : public Singleton<Resource> {
public:
  Resource();
  ~Resource();
  const sf::Texture& get_pix_tex() const { return m_pix_tex; }
private:
  sf::Texture m_pix_tex;
};