#pragma once

class Background {
public:
  Background();
  void draw(sf::RenderWindow& window);
private:
  uint8_t get_map(int32_t x, int32_t y) const;
  const std::vector<uint8_t>& m_map;
  sf::Sprite m_spr;
};