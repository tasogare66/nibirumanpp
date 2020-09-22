#include "stdafx.h"

#include "ConstParam.h"
#include "Random.h"
#include "Player.h"

#include "Camera.h"

void Camera::upd(const std::vector<Player*>& pls)
{
  auto p = pls[0]->get_pos();
  m_inr_center.x = std::clamp(fw::lerp(m_inr_center.x, p.x, 0.2f), -80.f, 80.f);
  m_inr_center.y = std::clamp(fw::lerp(m_inr_center.y, p.y, 0.2f), -140.f, 130.f);
  m_center = m_inr_center + this->upd_shake();
  m_top_left = m_center - Vec2f(120.f, 64.f);
  m_trs = -m_top_left;

  m_view_2d.reset({ 0.f, 0.f, const_param::SCR_WIDTH, const_param::SCR_HEIGHT });
}

const sf::View& Camera::update_view()
{
  m_view.reset({ 0.f, 0.f, const_param::SCR_WIDTH, const_param::SCR_HEIGHT });
  m_view.setCenter(m_center);
  return m_view;
}

Vec2f Camera::upd_shake()
{
  auto ofs = Vec2f();
  if (m_shake <= 0) return ofs;
  ofs.x = rng::range(-16.f, 16.f);
  ofs.y = rng::range(-16.f, 16.f);
  ofs *= m_shake;
  float fade = 0.95f;
  m_shake = m_shake * fade;
  return ofs;
}
