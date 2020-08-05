#include "stdafx.h"

#include "PlBullet.h"

PlBullet::PlBullet(const Vec2f pos, const Vec2f dir)
  : Entity({EntityType::PlBullet, pos})
  , m_dir(dir*150.f)
{
}

void PlBullet::init()
{
  this->attr_verlet();
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
