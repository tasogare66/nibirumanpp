#include "stdafx.h"

#include "Enemy.h"

Enemy::Enemy(const EntityArgs& args)
  : Entity(args)
{
}

void Enemy::update(float dt)
{
  if (m_appear_flag) {
    //self.spr = 464 + (self.elapsed / 0.166) % 4
    if (m_elapsed >= 2.f) {
      this->appear();
      m_appear_flag = false;
    }
  } else {
    this->upd_ene(dt);
  }
  m_elapsed += dt;
}
