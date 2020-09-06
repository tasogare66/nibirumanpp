#include "stdafx.h"

#include "EneDot.h"
#include "GameSeq.h"
#include "Player.h"
#include "Resource.h"
#include "ConstParam.h"
#include "Enemy.h"

Enemy::Enemy(const EntityArgs& args, uint32_t spr_ene)
  : Entity(EntityType::Enemy, args)
  , m_spr_ene(spr_ene)
{
  m_spr.setTexture(Resource::inst().get_spr_tex());
}

void Enemy::update(float dt)
{
  if (m_appear_flag) {
    this->spr8x8(464 + static_cast<uint32_t>(m_elapsed / 0.166f) % 4);
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
  : Enemy(args, 274)
{
  m_flag.on(EntityFlag::HaveDot);
  this->spr8x8(274);
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
    auto s = static_cast<uint32_t>(m_elapsed/(const_param::FRAME2SEC*4))%4;
    uint32_t animdir = dir.x > 0 ? 0 : 1;
    this->spr8x8(m_spr_ene + s + animdir * 4);
  }
}
