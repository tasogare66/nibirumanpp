#include "stdafx.h"

#include "ConstParam.h"
#include "Input.h"
#include "Camera.h"
#include "GameSeq.h"
#include "PlBullet.h"
#include "Force.h"
#include "Sound.h"

#include "Player.h"

Player::Player(const EntityArgs& args, const Entity* reticle, int32_t index)
  : Entity(EntityType::Player, args)
  , m_reticle(reticle)
  , m_index(index)
{
  m_flag.on(EntityFlag::Ally);
  m_colli_attr.on(Player::get_player_hit_mask(index));
  m_product_colli_attr = Player::get_generated_player_hit_mask(index);
}

void Player::init()
{
  this->attr_px();
}

void Player::update(float dt)
{
  //upd anim
  m_animcnt = static_cast<uint32_t>(m_elp/(const_param::FRAME2SEC*10))%4;
  m_elp += dt;
  if (not m_active) return;
  auto chara_dir = m_reticle->get_pos() - this->get_pos();
  m_animdir = (chara_dir.x > 0) ? 0 : 1;
  m_animdir |= ((chara_dir.y < 0) ? 2 : 0);
  if (this->check_dead()) return;
  this->upd_invincible(dt);
  this->upd_armslv(dt);

  Vec2f v;
  const auto& inputm = Input::inst();
  if (inputm.on(InputButton_Up)) v.y -= 1.0f;
  if (inputm.on(InputButton_Down)) v.y += 1.0f;
  if (inputm.on(InputButton_Left)) v.x -= 1.0f;
  if (inputm.on(InputButton_Right)) v.x += 1.0f;
  if (v.sqr_magnitude() >= 0.01f) {
    v.normalize();
    m_mov = v * (60 * dt);
  }

  //dash
  auto dashon = inputm.on(InputButton_Dash);
  if (this->get_flag().check(EntityFlag::Invincible) && m_invincible_time < 1.0f) dashon = false; //ignore start invincible
  if (m_dashst == 0) {
    if (m_coolt > 0.0f) {
      m_coolt -= dt;
      if (m_coolt <= 0.0f) { Sound::psfx(SfxId::DashCoolt, SndChannel::SFX3); }
    } else {
      if (dashon) {
        auto v = m_reticle->get_pos() - m_pos;
        auto len = v.magnitude();
        if (len > const_param::EPSILON) {
          v /= len;
          Camera::inst().req_shake(0.2f);
          this->set_vel_force(v * 6.8f);
          m_dashvec = v;
          m_dashvec_old = v;
          m_dasht = m_dash_limit;
          m_dashst = 1;
        }
      }
    }
  } else if (m_dashst == 1) {
    m_dasht -= dt;
    m_dashvec = m_pos - m_old_pos;
    m_dashvec.normalize();
    if (const_param::framecnt(m_elp) % 10 == 0) {
      Sound::psfx(SfxId::Dash, SndChannel::SFX0);
    }

    if (m_dash_limit - m_dasht < 0.1f || (dashon && m_dasht >= 0)) {
      this->set_vel_force(m_dashvec * 6.8f);
      if (v.x != 0.0f || v.y != 0.0f) {
        auto ang = Vec2f::angle(m_dashvec, v);
        auto dif = std::min(ang, fw::deg2rad(1.0f));
        if (Vec2f::cross(m_dashvec, v) < 0.0f) {
          dif = -dif;
        }
        m_dashvec.set_rotate(dif);
      }
      auto th = Vec2f::dot(m_dashvec, m_dashvec_old);
      if (th <= 0.5f) { //cos60°
        new ForceF(m_pos, this->get_product_colli_attr(), this->dash_pow());
        Camera::inst().req_shake(0.3f);
      }
      else {
        new ForceD(m_pos, this->get_product_colli_attr());
      }
    }
    else {
      new ForceF(m_pos, this->get_product_colli_attr(), this->dash_pow());
      Camera::inst().req_shake(0.3f);
      this->set_vel_force(m_dashvec * 0.2f);
      m_flwt = m_dash_followt;
      m_coolt = m_dash_coolt;
      m_dashst = 2;
    }
    m_dashvec_old = m_dashvec;
  }
  else if (m_dashst >= 2) { //invincible
    m_flwt -= dt;
    this->lim_vel_force(0.2f);
    if (m_flwt < 0.0f) {
      m_dashst = 0;
    }
  }

  //shot
  if (m_shot_repeat > 0) --m_shot_repeat;
  if (inputm.on(InputButton_Shot) && not this->is_dashing()) {
    if (m_shot_repeat <= 0) {
      auto v = m_reticle->get_pos() - m_pos;
      const auto d = v.magnitude();
      if (d > const_param::EPSILON) {
        v /= d;
        auto mzl = m_pos + v*4.f;
        new PlBullet(mzl, v, this);
        if (m_armslv >= 1) {
          constexpr auto ang = fw::deg2rad(20.f);
          auto c = std::cos(ang);
          auto s = std::sin(ang);
          new PlBullet(mzl, Vec2f(v.x * c - v.y * s, v.y * c + v.x * s), this);
          new PlBullet(mzl, Vec2f(v.x * c + v.y * s, v.y * c - v.x * s), this);
        }
        Sound::psfx(SfxId::PlShot, SndChannel::SFX0);

        constexpr int32_t shot_repeat_cnt[] = { 4, 6, 5 };
        m_shot_repeat = shot_repeat_cnt[std::clamp<size_t>(m_armslv,0,fw::array_size(shot_repeat_cnt)-1)];
      }
    }
  }
#if DEBUG&0
  if (inputm.trig(InputButton_Dash)) {
    //new ForceF(m_pos, this->get_product_colli_attr());
    new ForceD(m_pos, this->get_product_colli_attr());
  }
#endif
}

void Player::draw(sf::RenderWindow& window)
{
  auto sprid = 400 + m_animdir * 16 + m_animcnt;
  if (this->get_flag().check(EntityFlag::Invincible) && m_active) {
    uint32_t r = static_cast<uint32_t>(m_invincible_time / (const_param::FRAME2SEC * 6));
    if (r % 2 == 0) sprid = 267;
  }

  this->spr8x8(sprid);
  Entity::draw(window);
}

bool Player::check_dead()
{
  if (m_hit_mask.check(HitMask::Enemy)) {
    m_hit_mask.off(HitMask::Enemy);
    if (not this->is_dash()) {
      Camera::inst().req_shake(1.4f);
      if (GameSeq::decriment_life(m_index) > 0) {
        new ForceF(m_pos, this->get_product_colli_attr());
      }
      this->reset_dash();
      this->set_invincible();
      GameSeq::reset_multiplier(m_index);
      GameSeq::inst().reduceDiff(300);
      return true;
    }
  }
  return false;
}

void Player::set_invincible()
{
  m_flag.on(EntityFlag::Invincible);
  m_invincible_time = 0.0f;
}

void Player::upd_invincible(float dt)
{
  if (m_flag.check(EntityFlag::Invincible)) {
    if (m_invincible_time > 3.0f) { //3秒
      m_flag.off(EntityFlag::Invincible);
    }
    m_invincible_time += dt;
  }
}

void Player::add_armslv()
{
  m_armslv = std::min(m_armslv + 1, 3);
  m_armstime = 18.0f;
}

void Player::upd_armslv(float dt)
{
  if (m_armstime > 0.0f) {
    m_armstime -= dt;
  } else {
    m_armslv = 0;
  }
}
