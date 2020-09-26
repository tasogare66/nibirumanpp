#pragma once

namespace gmutil {
  void draw_text(sf::RenderWindow& window, sf::Text& text, float ix, float iy, int32_t icol);
  float draw_text_center(sf::RenderWindow& window, sf::Text& text, float iy, int32_t icol);
}