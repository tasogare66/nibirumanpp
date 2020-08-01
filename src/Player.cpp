#include "stdafx.h"

#include "ConstParam.h"
#include "Input.h"
#include "PlBullet.h"
#include "Player.h"

Player::Player()
  : Entity()
{
  this->attr_px();
}

void Player::update(float dt)
{
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
	  Vec2f v(1.0f, 0.0f);
	  //local v = self.reticle.pos - self.pos
	  const auto d = v.magnitude();
	  if (d > const_param::EPSILON) {
		//new PlBullet();
		//  ObjLstA : add(self.pos.x, self.pos.y, PlBullet, { dir = v:Div(d) })
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
