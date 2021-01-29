#include "stdafx.h"

#include "ConstParam.h"
#include "Input.h"
#include "Camera.h"
#include "GameSeq.h"
#include "Reticle.h"
#include "PlBullet.h"
#include "Force.h"
#include "Sound.h"
#include "SaveData.h"

#include "Player.h"

Player::Player(const EntityArgs& args, const Reticle* reticle, int32_t index)
  : Entity(EntityType::Player, args)
  , m_reticle(reticle)
  , m_index(index)
{
  m_flag.set(EntityFlag::Ally);
  m_colli_attr.set(Player::get_player_hit_mask(index));
  m_product_colli_attr = Player::get_generated_player_hit_mask(index);
}

void Player::init()
{
  this->attr_px();
}

void Player::set_animdir(const Vec2f& in_dir)
{
  m_animdir = (in_dir.x > 0) ? 0 : 1;
  m_animdir |= ((in_dir.y < 0) ? 2 : 0);
}

void Player::update(float dt)
{
  const auto& [inputp, inputd] = Input::inst().input_pair(m_index);;

  //upd anim
  m_animcnt = static_cast<uint32_t>(m_elp/(const_param::FRAME2SEC*10))%4;
  m_elp += dt;
  if (not m_active) return;
  Vec2f chara_dir = m_chara_dir_old;
  if (m_reticle && !inputp.m_use_joystick) {
    chara_dir = m_reticle->get_pos() - this->get_pos();
  } else if (inputd.m_analog_r.sqr_magnitude() > 0.1f) {
    chara_dir = inputd.m_analog_r;
  } else if (inputd.m_analog_l.sqr_magnitude() > 0.1f) {
    chara_dir = inputd.m_analog_l;
  }
  m_chara_dir_old = chara_dir;
  this->set_animdir(this->is_dashing() ? m_dashvec : chara_dir);
  if (this->check_dead()) return;
  this->upd_invincible(dt);
  this->upd_armslv(dt);

  Vec2f v = inputd.m_analog_l;
  auto len = v.magnitude();
  if (len > 0.2f) {
    if (len > 1.0f) v.normalize();
    m_mov = v * (60 * dt);
  }

  //dash
  auto can_dash = inputd.on(InputButton_Dash) | inputd.on(InputButton_PadDash);
  if (this->get_flag().test(EntityFlag::Invincible) && m_invincible_time < 1.0f) can_dash = false; //ignore start invincible
  if (m_dashst == 0) {
    if (m_coolt > 0.0f) {
      m_coolt -= dt;
      if (m_coolt <= 0.0f) { Sound::psfx(SfxId::DashCoolt, SndChannel::SFX3); }
    } else {
      if (can_dash) {
        Vec2f v;
        if (inputd.on(InputButton_PadDash)) {
          if (inputd.m_analog_l.magnitude() >= m_analog_threshold) {
            v = inputd.m_analog_l;
          }else if (inputd.m_analog_r.magnitude() >= m_analog_threshold) {
            v = inputd.m_analog_r;
          } else {
            v = m_chara_dir_old;
          }
        }
        else if (inputd.on(InputButton_Dash) && m_reticle) {
          v = m_reticle->get_pos() - m_pos;
        }

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

    if (m_dash_limit - m_dasht < 0.1f || (can_dash && m_dasht >= 0)) {
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
  if (m_shot_repeat <= 0 && not this->is_dashing()) {
    Vec2f v;
    if (inputd.m_analog_r.magnitude() >= m_analog_threshold) {
      v = inputd.m_analog_r; //analog_r優先
    } else if (inputd.on(InputButton_Shot) && m_reticle) {
      v = m_reticle->get_pos() - m_pos;
    }
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
      m_shot_repeat = shot_repeat_cnt[std::clamp<size_t>(m_armslv, 0, fw::array_size(shot_repeat_cnt) - 1)];
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
  if (this->get_flag().test(EntityFlag::Invincible) && m_active) {
    uint32_t r = static_cast<uint32_t>(m_invincible_time / (const_param::FRAME2SEC * 6));
    if (r % 2 == 0) sprid = 267;
  }
  this->spr8x8(sprid);
  Entity::draw(window);

  //gauge
  if (this->is_dash() || m_coolt > 0.f) {
    auto c = m_pos - Vec2f(4.f,4.f);
    float len = 10.0f;
    c.y = c.y - 2.0f;
    c.x = c.x - (len - 8.0f) / 2.0f;
    float r = 0.f;
    int col = 6;
    if (this->is_dash()) {
      r = m_dasht / m_dash_limit;
      col = 11;
    }
    else {
      r = 1.f - m_coolt / m_dash_coolt;
    }
    sf::RectangleShape line(sf::Vector2f(len, 1));
    line.setPosition(c);
    line.setFillColor(const_param::ticcol(15));
    window.draw(line);
    line.setSize(Vec2f(len*r, 1));
    line.setFillColor(const_param::ticcol(col));
    window.draw(line);
  }
}

bool Player::check_dead()
{
  if (m_hit_mask.test(HitMask::Enemy)) {
    m_hit_mask.reset(HitMask::Enemy);

    bool is_invincible = this->is_dash();
#if DEBUG
    is_invincible |= SaveDataMng::deb_conf().m_invincible;
#endif
    if (not is_invincible) {
      Camera::inst().req_shake(1.4f);
      if (GameSeq::decriment_life() >= 0) {
        new ForceF(m_pos, this->get_product_colli_attr());
      } else {
        this->set_gone_state(); //死亡
      }
      this->reset_dash();
      this->set_invincible();
      GameSeq::reset_multiplier();
      GameSeq::inst().reduceDiff(300);
      return true;
    }
  }
  return false;
}

void Player::set_invincible()
{
  this->Entity::set_invincible(true);
  m_invincible_time = 0.0f;
}

void Player::upd_invincible(float dt)
{
  if (m_flag.test(EntityFlag::Invincible)) {
    if (m_invincible_time > 3.0f) { //3秒
      this->Entity::set_invincible(false);
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

void Player::set_gone_state()
{
  m_flag.set(EntityFlag::DefaultMask, false);
  m_is_gone = true;
}
