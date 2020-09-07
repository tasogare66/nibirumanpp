#include "stdafx.h"

#include "ConstParam.h"
#include "Input.h"
#include "PlBullet.h"
#include "Player.h"

Player::Player(const EntityArgs& args, const Entity* reticle)
  : Entity(EntityType::Player, args)
  , m_reticle(reticle)
{
  m_flag.on(EntityFlag::Ally);
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
  //if self:check_dead() then return end
  //self : check_invincible(dt)
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
  		new PlBullet(m_pos, v/d);
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
