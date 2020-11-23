#pragma once
#include <functional>
#include "Vec2.h"

namespace gmutil {
  void draw_text(sf::RenderWindow& window, sf::Text& text, float ix, float iy, int32_t icol);
  float calc_text_center_x(const sf::Text& text);
  float draw_text_center(sf::RenderWindow& window, sf::Text& text, float iy, int32_t icol);
  void random_circle(int32_t num, float st_r, float ed_r, std::function<void(Vec2f)> func);
}