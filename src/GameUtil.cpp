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
  float calc_text_center_x(const sf::Text& text) {
    const auto& bound = text.getLocalBounds();
    return (const_param::WND_WIDTH - bound.width) / 2.0f;
  }
  float draw_text_center(sf::RenderWindow& window, sf::Text& text, float iy, int32_t icol) {
    float x = calc_text_center_x(text);
    draw_text(window, text, x, iy, icol);
    return x;
  }

  void draw_circle(sf::RenderWindow& window, Vec2f center, float radius)
  {
    sf::CircleShape shape;
    shape.setRadius(radius);
    center = -center + Vec2f(radius,radius);
    shape.setOrigin(center);
    shape.setFillColor(sf::Color(0));
    shape.setOutlineThickness(0.5f);
    window.draw(shape);
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