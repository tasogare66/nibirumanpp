#include "stdafx.h"

#include "EneDot.h"
#include "GameSeq.h"
#include "Player.h"
#include "Enemy.h"

Enemy::Enemy(const EntityArgs& args)
  : Entity(EntityType::Enemy, args)
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

//enemy snake
EneSnake::EneSnake(const EntityArgs& args)
  : Enemy(args)
{
  m_flag.on(EntityFlag::HaveDot);
}

void EneSnake::appear()
{
  this->attr_px();
}

void EneSnake::upd_ene(float dt)
{
  if (!m_tgt) m_tgt = GameSeq::inst().get_player_for_enemy();
  if (m_tgt) {
    auto dir = m_tgt->get_pos() - this->get_pos();
    dir.normalize();
    auto len = 9.f * dt * GameSeq::inst().getDifV(1.f, 3.f);
    m_mov += (dir * len);
      //local s = (self.elapsed//(FRAME2SEC*4))%4
      //  local animdir = dir.x > 0 and 0 or 1
      //  self.spr = self.spr_ene + s + animdir * 4
  }
}
