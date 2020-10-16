#include "stdafx.h"

#include "ConstParam.h"
#include "Resource.h"
#include "PtclLst.h"
#include "Random.h"
#include "LuaScript.h"

#include "Boss.h"

Boss::Boss(const EntityArgs& args, uint32_t spr_ene)
  : Enemy(args, spr_ene)
{
}

//level_0
BossBaby::BossBaby(const EntityArgs& args)
  : Boss(args, 328)
{
  this->set_radius(16);
  this->set_mass(5);
  m_health_max = 500;
  m_health = m_health_max;
  //spr
  m_spr_body.setTexture(Resource::inst().get_spr_tex());
  //circle
  m_circle.setRadius(m_radius);
  m_circle.setFillColor(sf::Color(0));
  m_circle.setOutlineThickness(0.5f);
  m_circle.setOutlineColor(const_param::ticcol(9));
  //script
  m_script = scr::create_lua_boss_sequence(std::string_view("update_baby"), this);
}

void BossBaby::appear()
{
  this->attr_px();
}

void BossBaby::update(float dt)
{
  Enemy::update(dt);
  //self:upd_blink(dt)
    //self.animcnt = (self.elapsed//(FRAME2SEC*20)%2)
    //  if (self.elapsed//(FRAME2SEC*14)%2==0) then --random offset
    //	self.dspofs:Set(2 - random(4), 2 - random(4))
    //	end
  if (m_appear_flag) {
    PtclLst::add_sqr(m_pos, 2, m_radius+6.0f);
    //if self.elapsed//FRAME2SEC%20==0 then psfx(11,'A-4',20,3) end
  }
}

void BossBaby::upd_ene(float dt)
{
  m_arms_timer += dt;

  this->arms0(0.2f);

  if (m_script) {
    m_script->exec(dt);
  }
}

void BossBaby::draw(sf::RenderWindow& window)
{
  if (m_appear_flag) return;

    m_circle.setOrigin(-m_pos.x + m_radius, -m_pos.y + m_radius);
  window.draw(m_circle);
}


void BossBaby::arms0(float t, int32_t num, float ofs)
{
  ofs = rng::randf(ofs);
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
