#include "stdafx.h"

#include "Player.h"

#include "PlBullet.h"

PlBullet::PlBullet(const Vec2f pos, const Vec2f dir, const Player* owner)
  : Entity(EntityType::PlBullet, { pos, dir*150.f })
  , m_owner(owner)
{
  this->spr8x8(496);
  this->apply_angle();
  m_colli_attr.set(m_owner->get_product_colli_attr());
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
