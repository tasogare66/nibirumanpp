#include "stdafx.h"

#include "ConstParam.h"
#include "Input.h"
#include "Camera.h"
#include "GameSeq.h"
#include "PlBullet.h"
#include "Force.h"

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
  //self : upd_armslv(dt)

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
  //shot
  if (m_shot_repeat > 0) --m_shot_repeat;
  if (inputm.on(InputButton_Shot) && not this->is_dashing()) {
    if (m_shot_repeat <= 0) {
      auto v = m_reticle->get_pos() - m_pos;
      const auto d = v.magnitude();
      if (d > const_param::EPSILON) {
        v /= d;
        new PlBullet(m_pos+v*4.f, v, this);
        if (m_armslv >= 1) {
          //	local ang = math.rad(20)
          //	local c, s = cos(ang), sin(ang)
          //	ObjLstA:add(self.pos.x, self.pos.y, PlBullet, { dir = Vec2.new(v.x * c - v.y * s,v.y * c + v.x * s) })
          //	ObjLstA : add(self.pos.x, self.pos.y, PlBullet, { dir = Vec2.new(v.x * c + v.y * s,v.y * c - v.x * s) })
        }
        //	psfx(3, 'C-3', 9, 0)
        constexpr int32_t shot_repeat_cnt[] = { 4, 6, 5 };
        m_shot_repeat = shot_repeat_cnt[std::clamp<size_t>(m_armslv,0,fw::array_size(shot_repeat_cnt)-1)];
      }
    }
  }
#if DEBUG&1
  if (inputm.trig(InputButton_Dash)) {
    new ForceF(m_pos);
  }
#endif
}

void Player::draw(sf::RenderWindow& window)
{
  auto sprid = 400 + m_animdir * 16 + m_animcnt;
    //if self:check_flag(Flag_Invincible) and self.active then
    //  local r = self.invincible_time // (FRAME2SEC*6)
    //  if r % 2 == 0 then self.spr = 267 end
    //	end

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
        //  ObjLstA : add(self.pos.x, self.pos.y, ForceF)
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
