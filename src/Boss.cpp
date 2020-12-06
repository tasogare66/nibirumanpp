﻿#include "stdafx.h"

// warning対処
#pragma warning( push )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4189 )
#include <lua.hpp>
#include <LuaIntf/LuaIntf.h>
#pragma warning( pop )

#include "ConstParam.h"
#include "Resource.h"
#include "PtclLst.h"
#include "Random.h"
#include "LuaScript.h"
#include "Camera.h"
#include "GameUtil.h"
#include "EneDot.h"
#include "Sound.h"
#include "ModeMng.h"
#include "ModeGame.h"

#include "Boss.h"

BossParts::BossParts(const EntityArgs& args, uint32_t spr_ene)
  : Enemy(args, spr_ene)
{
}

void BossParts::set_circle_radius(float radius)
{
  m_circle.setRadius(radius);
  m_circle.setFillColor(sf::Color(0));
  m_circle.setOutlineThickness(0.5f);
  m_circle.setOutlineColor(const_param::ticcol(9));
}

void BossParts::draw_circle(sf::RenderWindow& window)
{
  float radius = m_circle.getRadius();
  m_circle.setOrigin(-m_pos.x + radius, -m_pos.y + radius);
  window.draw(m_circle);
}


//root
Boss::Boss(const EntityArgs& args, uint32_t spr_ene)
  : BossParts(args, spr_ene)
{
}

Boss::~Boss()
{
  this->remove_bossrf();
}

void Boss::dead_base()
{
  Camera::inst().req_shake(2.0f);
  Enemy::dead();
  Sound::psfx(SfxId::Force, SndChannel::SFX3);
}

//ModeGameに登録
void Boss::set_bossrf()
{
  if (auto* mg = ModeMng::inst().get_current_mode<ModeGame>()) {
    mg->set_bossrf(this);
  }
}
//ModeGameから削除
void Boss::remove_bossrf()
{
  if (auto* mg = ModeMng::inst().get_current_mode<ModeGame>()) {
    mg->remove_bossrf(this);
  }
}
//bossが存在するか
bool Boss::is_exist_boss() {
  if (auto* mg = ModeMng::inst().get_current_mode<ModeGame>()) {
    return mg->is_exist_boss();
  }
  return false;
}

Vec2f Boss::get_dir(Vec2f tgt) {
  auto dir = tgt - m_pos;
  auto len = dir.magnitude();
  if (len > const_param::EPSILON) {
    return dir / len;
  }
  return dir;
}

void Boss::update(float dt)
{
  this->set_bossrf();
  Enemy::update(dt);
  if (m_appear_flag) {
    PtclLst::add_sqr(m_pos, 2, m_radius + 6.0f);
    if (const_param::framecnt(m_elapsed) % 20 == 0) Sound::psfx(SfxId::BossAp, SndChannel::SFX3);
  }
}

void Boss::upd_ene_base(float dt)
{
  if (m_script) {
    m_script->exec(dt);
  }
  m_elapsed += dt;
}

void Boss::move_to(float px,float py,float spd)
{
  //m_mov += this->get_dir({ px,py }) * spd * m_dt;
  Vec2f tgt(px,py);
  auto dir = tgt - m_pos;
  auto len = dir.magnitude();
  if (len > const_param::EPSILON) {
    dir /= len;
  }
  len = std::min(len,spd*m_dt);
  m_mov +=  dir*len;
  m_dir = dir; //向きも設定
}

void Boss::draw_info1(sf::RenderWindow& window) const
{
  float hr = static_cast<float>(m_health) / m_health_max;
  const float len{ 150.f }, len2{ 150.f * hr };
  float sx = (const_param::SCR_WIDTH - len) / 2.f;
  constexpr float y = 125.f;
  sf::RectangleShape line0(Vec2f(len,1));
  line0.setFillColor(const_param::ticcol(3));
  sf::RectangleShape line1(Vec2f(len2, 1));
  line1.setFillColor(const_param::ticcol(11));
  for (int i = 1; i <= 3; ++i) {
    line0.setPosition(sx, y+i);
    window.draw(line0);
    line1.setPosition(sx, y+i);
    window.draw(line1);
    sx += 1.0f;
  }
}

void Boss::draw_info2(sf::RenderWindow& window, sf::Text& text) const
{
  if (m_appear_flag) {
    text.setString("WARNING");
    constexpr float scl = const_param::SCR_SCALE;
    float y = 50.0f*scl;
    float x = gmutil::calc_text_center_x(text);
    float ofsx = rng::range_int(-6, 6)*scl;
    float ofsy = rng::range_int(-6, 6)*scl;
    gmutil::draw_text(window, text, x+ofsx, y+ofsy, rng::range_int(1,15));
    gmutil::draw_text(window, text, x, y, 6);
  }
}


//level_0
BossBaby::BossBaby(const EntityArgs& args)
  : Boss(args, 328)
{
  this->set_radius(16);
  this->set_mass(5);
  m_score = 5000;
  m_health_max = 500;
  m_health = m_health_max;
  //spr
  m_spr_body.setTexture(Resource::inst().get_spr_tex());
  //circle
  this->set_circle_radius(m_radius);
  //script
  m_script.reset(scr::create_lua_boss_sequence(std::string_view("update_baby"), this));
  m_script->reset_thread();
}

void BossBaby::appear()
{
  this->attr_px();
}

void BossBaby::update(float dt)
{
  this->update_dt(dt);
  Boss::update(dt);
  this->upd_blink(dt);
  m_animcnt = static_cast<uint32_t>(m_elapsed/const_param::FRAME2SEC*20)%2;
  if (static_cast<uint32_t>(m_elapsed/(const_param::FRAME2SEC*14))%2==0) { //random offset
    m_dspofs.set(rng::range(-2.f, 2.f), rng::range(-2.f, 2.f));
  }
}

void BossBaby::upd_ene(float dt)
{
  m_arms_timer += dt;
  this->upd_ene_base(dt);
}

void BossBaby::draw(sf::RenderWindow& window)
{
  if (m_appear_flag) return;

  if (this->is_blink()) {
    this->spr8x8(332, 4, 4);
    this->Enemy::draw(window);
  } else {
    this->spr8x8(m_spr_ene, 4, 4);
    this->Enemy::draw(window);

    auto p2 = m_pos + m_dspofs;
    this->spr8x8(392+ m_animcnt * 4, 4, 4);
    m_spr.setPosition(p2);
    window.draw(m_spr);
  }
  //circle
  this->draw_circle(window);
}

void BossBaby::dead()
{
  auto num = 300;
  gmutil::random_circle(num, 0.0f, 70.0f, [this](Vec2f p) { new EneDot(m_pos+p); });
  gmutil::random_circle(4, 0.0f, m_radius, [this](Vec2f p) { PtclLst::add(m_pos+p, 6); });
  gmutil::random_circle(6, 3.0f, m_radius*2, [this](Vec2f p) { PtclLst::add(m_pos + p, 15); });
  Boss::dead_base();
  //GAME.boss = nil
}

void BossBaby::use_arms(int type, const LuaIntf::LuaRef& tbl)
{
  switch (type) {
  case 0:
  {
    float t = tbl["t"].value<float>();
    int32_t num = tbl.has("num") ? tbl["num"].value<int32_t>() : 10;
    float ofs = tbl.has("ofs") ? tbl["ofs"].value<float>() : 0.0f;
    this->arms0(t, num, ofs);
  }
  break;
  default:
    FW_ASSERT(0);
    break;
  }
}

void BossBaby::arms0(float t, int32_t num, float ofs)
{
  ofs = fw::deg2rad(ofs);
  if (m_arms_timer > t) {
    float radius = m_radius + 3.0f;
    float rad_step = 2.0f * static_cast<float>(M_PI) / num;
    for (int32_t i = 0; i < num; ++i) {
      auto rad = rad_step * i + ofs;
      auto c = std::cos(rad);
      auto s = std::sin(rad);
      new BossBullet(EntityArgs(Vec2f(m_pos.x+c*radius, m_pos.y+s*radius), Vec2f(c,s)));
    }
    m_arms_timer = fmod(m_arms_timer,t);
  }
}


//level_1
class WormChild final : public BossParts {
public:
  WormChild(const EntityArgs& args)
    : BossParts(args)
  {
    this->set_radius(10);
    this->set_mass(3);
    m_health = std::numeric_limits<int32_t>::max();
    m_flag.set(EntityFlag::IgnoreCollisionSameRoot);
    //circle
    this->set_circle_radius(m_radius);
  }
  virtual ~WormChild() = default;
  void draw(sf::RenderWindow& window) override final {
    this->draw_circle(window);
  }
private:
  void appear() {
    this->attr_px();
  }
  void upd_ene(float dt) override {
  }
};

BossWorm::BossWorm(const EntityArgs& args)
  : Boss(args, 328)
{
  this->set_radius(16);
  this->set_mass(5);
  //m_score = 5000;
  m_health_max = 500;
  m_health = m_health_max;
  m_flag.set(EntityFlag::IgnoreCollisionSameRoot);
  //circle
  this->set_circle_radius(m_radius);

  //child
  Entity* parent = this;
  for (size_t i = 1; i <= 16; ++i) {
    auto p = args.m_pos + Vec2f(-18.f*i,0.f);
    auto child = new WormChild(p);
    Entity::set_hierarchy(parent, child);
    parent = child;
  }
  //ik
  m_ik.awake(this);
  m_ik.get_root_chain()->set_target_enabled(false);
  //script
  m_script.reset(scr::create_lua_boss_sequence(std::string_view("update_worm"), this));
  m_script->reset_thread();
}

void BossWorm::appear()
{
  this->attr_px();
}

void BossWorm::update(float dt)
{
  Boss::update(dt);
  this->upd_blink(dt);
}

void BossWorm::upd_ene(float dt)
{
  m_arms_timer += dt;
  this->upd_ene_base(dt);
  //ik
  if (m_effector) m_effector->apply_effector();
  m_ik.update();
}

void BossWorm::draw(sf::RenderWindow& window)
{
  //circle
  this->draw_circle(window);
}

void BossWorm::dead()
{
  Boss::dead_base();
}

void BossWorm::use_arms(int type, const LuaIntf::LuaRef& tbl)
{
  switch (type) {
  case 0:
  {
    float t = tbl["t"].value<float>();
    this->arms0(t);
  }
  break;
  default:
    FW_ASSERT(0);
    break;
  }
}

void BossWorm::arms0(float t)
{
  if (m_arms_timer > t) {
    this->exec_or_lower([this](Entity* e) {
      auto dir(e->get_dir());
      if (dir.sqr_magnitude() <= const_param::EPSILON) return;
      auto f = dir.rotate(static_cast<float>(M_PI)/2.f);
      new BossArrow(EntityArgs(EntityDataId::BossArrow, e->get_pos()+f*e->get_radius(), f));
      f = dir.rotate(static_cast<float>(-M_PI)/2.f);
      new BossArrow(EntityArgs(EntityDataId::BossArrow, e->get_pos()+f*e->get_radius(), f));
    });
    m_arms_timer = fmod(m_arms_timer, t);
  }
}
