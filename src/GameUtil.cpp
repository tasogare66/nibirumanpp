#include "stdafx.h"

#include "ConstParam.h"

#include "GameUtil.h"

namespace gmutil {
  void draw_text(sf::RenderWindow& window, sf::Text& text, float ix, float iy, int32_t icol) {
    text.setFillColor(const_param::ticcol(icol));
    text.setPosition(ix, iy);
    window.draw(text);
  }
  float draw_text_center(sf::RenderWindow& window, sf::Text& text, float iy, int32_t icol) {
    auto bound = text.getLocalBounds();
    float x = (const_param::WND_WIDTH - bound.width) / 2.0f;
    draw_text(window, text, x, iy, icol);
    return x;
  }
}