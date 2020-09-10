#include "stdafx.h"

#include "EneDot.h"
#include "GameSeq.h"
#include "Player.h"
#include "Resource.h"
#include "ConstParam.h"
#include "Random.h"

#include "Enemy.h"

Enemy::Enemy(const EntityArgs& args, uint32_t spr_ene)
  : Entity(EntityType::Enemy, args)
  , m_spr_ene(spr_ene)
{
  this->spr8x8(464); //appear
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
}
void EneSnake::appear()
{
  this->attr_px();
  this->spr8x8(m_spr_ene);
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

//enemy grunt
EneGrunt::EneGrunt(const EntityArgs& args)
  : Enemy(args,288)
{
  m_flag.on(EntityFlag::HaveDot);
}
void EneGrunt::appear()
{
  this->attr_px();
  this->spr8x8(m_spr_ene);
}
void EneGrunt::upd_ene(float dt)
{
  m_rad += fw::deg2rad(120.f * dt);
  auto len = 0.24f * GameSeq::inst().getDifV(1.f, 2.5f);
  m_mov.x += std::cos(m_rad) * len;
  m_mov.y += std::sin(m_rad) * len;
  auto s = static_cast<uint32_t>(m_elapsed / (const_param::FRAME2SEC * 12)) % 4;
  uint32_t animdir = m_mov.x > 0 ? 0 : 1;
  this->spr8x8(m_spr_ene + s + animdir * 4);
}

//enemh hulk
EneHulk::EneHulk(const EntityArgs& args)
  : Enemy(args,304)
{
  m_health = 30;
  this->setmvtm();
  m_animdir = rng::rand_int(1, rng::Type::GAME);
}

void EneHulk::appear()
{
  this->attr_px();
  this->spr8x8(m_spr_ene);
}

void EneHulk::upd_ene(float dt)
{
  //self:upd_blink(dt)
  auto s = static_cast<uint32_t>(m_elapsed / (const_param::FRAME2SEC * 8)) % 2;
  this->spr8x8(m_spr_ene + m_animdir * 2 + s);

  const auto* tgt = GameSeq::inst().get_player_for_enemy();
  Vec2f dir;
  if (tgt) dir = tgt->get_pos() - m_pos;
  const float len = dir.magnitude();
  if (m_mvtm < 0.0f && len > const_param::EPSILON) {
    m_animdir = dir.x > 0 ? 0 : 1;
    dir /= len;
    this->add_vel_force(dir * 0.8f);
    this->setmvtm();
  }
  else {
    m_mvtm -= dt;
  }
}
void EneHulk::setmvtm()
{
  m_mvtm = rng::range(2.5f, GameSeq::inst().getDifV(6.f, 2.7f), rng::Type::GAME);
}
