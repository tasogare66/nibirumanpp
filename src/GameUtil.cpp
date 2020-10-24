#include "stdafx.h"

#include "ConstParam.h"
#include "Random.h"

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

  void random_circle(int32_t num, float st_r, float ed_r, std::function<void(Vec2f)> func) {
    float r0 = st_r / ed_r;
    for (int32_t i = 0; i < num; ++i) {
      float r = std::sqrt(rng::range(r0, 1)) * ed_r;
      float theta = rng::range(static_cast<float>(-M_PI), static_cast<float>(M_PI));
      func(Vec2f(r * std::cos(theta), r * std::sin(theta)));
    }
  }
}