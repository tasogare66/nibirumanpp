#include "stdafx.h"

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
}

Vec2f Camera::upd_shake()
{
  return Vec2f();
}