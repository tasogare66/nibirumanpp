#include "stdafx.h"

#include "EneDot.h"
#include "GameSeq.h"
#include "Player.h"
#include "Resource.h"
#include "ConstParam.h"
#include "Random.h"
#include "PtclLst.h"

#include "Enemy.h"

Enemy::Enemy(const EntityArgs& args, uint32_t spr_ene)
  : Entity(EntityType::Enemy, args)
  , m_spr_ene(spr_ene)
{
  m_colli_attr.on(HitMask::Enemy);
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
  auto cbfunc = [this](int32_t player_id) {
    GameSeq::add_score(player_id, m_score);
  };
  if (this->check_kill_by_generated_player(cbfunc)) {
    if (m_flag.check(EntityFlag::HaveDot)) {
      new EneDot(m_pos);
      //psfx(2, 'D-4', 20, 1)
    }
    PtclLst::add(m_pos, 15);
  } else {
    FW_ASSERT(0);
  }
}

//enemy snake
EneSnake::EneSnake(const EntityArgs& args)
  : Enemy(args, 274)
{
  m_flag.on(EntityFlag::HaveDot);
  m_score = 20;
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
  m_score = 10;
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

//enemy hulk
EneHulk::EneHulk(const EntityArgs& args)
  : Enemy(args,304)
{
  m_flag.on(EntityFlag::HaveDot);
  m_health = 30;
  m_exp_resi = 15;
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

//enemy arrow
EneArrow::EneArrow(const EntityArgs& args, uint32_t spr_ene)
  : Enemy(args,spr_ene)
{
  m_flag.on(EntityFlag::HaveDot);
  m_dir = args.m_dir;
  const auto l = m_dir.magnitude();
  if (l < const_param::EPSILON) {
    m_dir.set(0.f,-1.f);
  }
  else {
    m_dir /= l;
  }
  m_speed = GameSeq::inst().getDifV(45, 60);
}

void EneArrow::appear()
{
  this->attr_ene_bullet();
  this->spr8x8(m_spr_ene);
  this->apply_angle();
}

void EneArrow::upd_ene(float dt)
{
  m_mov = m_dir * m_speed * dt;
  auto s = static_cast<uint32_t>(m_elapsed / (const_param::FRAME2SEC * 8)) % 2;
  this->spr8x8(m_spr_ene + s*2);
}

bool EneArrow::hit_wall(const Vec2f& dir)
{
  m_dir = dir;
  this->apply_angle();
  return false;
}

void EneArrow::apply_angle()
{
  auto ang = std::atan2(m_dir.y, m_dir.x);
  m_spr.setRotation(fw::rad2deg(ang));
}

//enemy arrow2
EneArrow2::EneArrow2(const EntityArgs& args)
  : EneArrow(args, 384)
{}

bool EneArrow2::hit_wall(const Vec2f&)
{
  m_dir *= -1.f;
  this->apply_angle();
  return false;
}

//enemy sphe
EneSphe::EneSphe(const EntityArgs& args)
  : Enemy(args, 308)
{
  m_flag.on(EntityFlag::HaveDot);
  m_health = 2;
  m_exp_resi = 2;
  //self.drw = self.drw_blink
  m_rotr = m_pos.magnitude();
  m_rdir = args.m_param0 > 0.0f ? 1.0f : -1.0f;
  m_speed = GameSeq::inst().getDifV(50, 60);
}

void EneSphe::appear()
{
  this->attr_ene_bullet();
  this->spr8x8(m_spr_ene);
}

void EneSphe::upd_ene(float dt)
{
  //self:upd_blink(dt)
  auto s = static_cast<uint32_t>(m_elapsed / (const_param::FRAME2SEC * 4)) % 2;
  this->spr8x8(m_spr_ene + s); //FIXME:移動向きでsprite変えていない  

  if (m_rotr > const_param::EPSILON) {
    auto rad = m_speed * dt / m_rotr * m_rdir;
    m_mov = m_pos.rotate(rad) - m_pos;
  }
}

//boss bullet
BossBullet::BossBullet(const EntityArgs& args)
  : Enemy(args, 336)
  , m_speed(50.f)
{
  m_dir = args.m_dir;
  m_dir.normalize();
}

void BossBullet::init()
{
  this->attr_ene_bullet();
  this->spr8x8(m_spr_ene);
}

void BossBullet::update(float dt)
{
  m_mov = m_dir * m_speed * dt;
  auto s = static_cast<uint32_t>(m_elapsed / (const_param::FRAME2SEC * 8)) % 3;
  this->spr8x8(m_spr_ene + s);
  m_elapsed += dt;
}

bool BossBullet::hit_wall(const Vec2f&)
{
  this->del();
  return true;
}
