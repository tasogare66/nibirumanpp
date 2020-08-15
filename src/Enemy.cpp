#include "stdafx.h"

#include "EneDot.h"
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

void Enemy::dead()
{
  //GAME:add_score(self.score)
  if (m_flag.check(EntityFlag::HaveDot)) {
    new EneDot(m_pos);
    //psfx(2, 'D-4', 20, 1)
  }
}
