#include "stdafx.h"

#include "Camera.h"
#include "Resource.h"
#include "ConstParam.h"
#include "GameUtil.h"

#include "Background.h"

Background::Background()
  : m_map(Resource::inst().get_map())
{
  m_spr.setTexture(Resource::inst().get_spr_tex());

  constexpr auto lvrad = const_param::LvRadius;
  m_stg_circle.setRadius(lvrad);
  m_stg_circle.setOrigin(lvrad, lvrad);
  m_stg_circle.setFillColor(sf::Color(0));
  m_stg_circle.setOutlineThickness(0.5f);
  m_stg_circle.setPointCount(m_stg_circle.getPointCount()*2);
}

void Background::draw(sf::RenderWindow& window)
{
  const auto& view = Camera::inst().get_view();
  const auto& vsz = view.getSize();
  const int nx = static_cast<int>(std::ceil(vsz.x / 8.0f))+1;
  const int ny = static_cast<int>(std::ceil(vsz.y / 8.0f))+1;

  Vec2f topleft = Camera::inst().get_top_left();
  const int sx = static_cast<int>(std::floor(topleft.x / 8.0f));
  const int sy = static_cast<int>(std::floor(topleft.y / 8.0f));

  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      auto mx = x + sx;
      auto my = y + sy;
      auto id = get_map(mx, my);

      m_spr.setPosition(mx*8.f,my*8.f);

      auto rect = Resource::get_spr_rect(id);
      m_spr.setTextureRect(rect);

      window.draw(m_spr, sf::BlendNone);
    }
  }

  //circle
  window.draw(m_stg_circle);
#if DEBUG
  gmutil::draw_circle(window, {0.f,0.f}, 100.f);
#endif
}

uint8_t Background::get_map(int32_t x, int32_t y) const
{
  while (x < 0) x += 240;
  while (y < 0) y += 136;
  x %= 240;
  y %= 136;
  int32_t pt = 240 * y + x;
  if (pt >= 0 && pt < m_map.size()) return m_map[pt];
  FW_ASSERT(0);
  return 0;
}
