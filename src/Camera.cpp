﻿#include "stdafx.h"

#include "ConstParam.h"
#include "Random.h"
#include "Player.h"
#include "GameSeq.h"

#include "Camera.h"

void Camera::upd()
{
  Vec2f p;
  Vec2f min(FLT_MAX,FLT_MAX);
  Vec2f max(-FLT_MAX,-FLT_MAX);
  const auto& gameseq = GameSeq::inst();
  const auto& pl_ids = gameseq.get_alive_player_ids();
  if (pl_ids.size() > 0) {
    for (const auto& pid : pl_ids) {
      const auto& pl = gameseq.get_player_entity(pid);
      const auto& ppos = pl->get_pos();
      p += ppos;
      min.x = std::min(min.x,ppos.x);
      min.y = std::min(min.y,ppos.y);
      max.x = std::max(max.x, ppos.x);
      max.y = std::max(max.y, ppos.y);
    }
    p *= (1.0f/ pl_ids.size());
  }
  Vec2f margin(70.f, 70.0f);
  Vec2f size = max - min + margin;
  float scl = 1.0f;
  constexpr float sclmax = 2.3f;
  scl = std::max(scl, size.x/const_param::SCR_WIDTH);
  scl = std::max(scl, size.y / const_param::SCR_HEIGHT);
  scl = std::min(scl, sclmax);
  m_inr_center.x = std::clamp(fw::lerp(m_inr_center.x, p.x, 0.08f), -80.f, 80.f);
  m_inr_center.y = std::clamp(fw::lerp(m_inr_center.y, p.y, 0.08f), -136.f, 136.f);
  m_center = m_inr_center + this->upd_shake();
  m_view_scale = fw::lerp(m_view_scale, scl, 0.1f);

  m_view_2d.reset({ 0.f, 0.f, const_param::SCR_WIDTH, const_param::SCR_HEIGHT });
}

const sf::View& Camera::update_view()
{
  m_view.reset({ 0.f, 0.f, const_param::SCR_WIDTH*m_view_scale, const_param::SCR_HEIGHT*m_view_scale });
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
