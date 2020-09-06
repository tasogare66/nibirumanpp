#include "stdafx.h"

#include "EneDot.h"

EneDot::EneDot(const EntityArgs& args)
  : Entity(EntityType::Dot, args)
{
  this->spr8x8(m_ene_spr);
}

void EneDot::init()
{
  m_flag.on(EntityFlag::Ally);
  this->attr_ene_dot();
}

void EneDot::update(float dt)
{
  if (m_capture_pl) {
    auto t = m_captime * m_captime * m_captime;
    this->set_position(fw::lerp(m_pos, m_capture_pl->get_pos(), t));
    auto ttl = 1.f;
    m_captime = std::min(m_captime + dt / ttl, 1.f);
  }
  else if (m_timer < 0) {
    this->del();
    return;
  }
  this->spr8x8(m_ene_spr + static_cast<uint32_t>(m_timer / 0.333f) % 2);
  m_timer -= dt;
}

void EneDot::hitcb(const Entity* o, const Vec2f& dir, float dist)
{
  if (dist < m_radius) {
    // GAME:add_score(self.score)
    // GAME:add_multiplier()
    // GAME:reduceDiff(0.5)
    //psfx(4,'F-6',5,2)
    this->del();
  }
  else {
    m_capture_pl = o;
  }
}
