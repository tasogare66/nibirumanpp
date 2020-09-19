#include "stdafx.h"

#include "Player.h"

#include "PlBullet.h"

PlBullet::PlBullet(const Vec2f pos, const Vec2f dir, const Player* owner)
  : Entity(EntityType::PlBullet, { pos })
  , m_dir(dir*150.f)
  , m_owner(owner)
{
  this->spr8x8(496);
  m_ang = std::atan2(m_dir.y, m_dir.x);
  m_spr.setRotation(fw::rad2deg(m_ang));
  m_colli_attr.on(m_owner->get_product_colli_attr());
}

void PlBullet::init()
{
  this->attr_bullet();
}

void PlBullet::update(float dt)
{
  m_elp += dt;
  if (m_elp >= 1.0f) {
    this->del();
    return;
  }
  m_mov = m_dir * dt;
}

bool PlBullet::hit_wall(const Vec2f&)
{
  this->del();
  return true;
}
