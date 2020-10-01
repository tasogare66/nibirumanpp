#include "stdafx.h"

#include "ConstParam.h"

#include "Force.h"

ForceF::ForceF(const Vec2f pos, const HitMask colli_attr, float power)
  : Entity(EntityType::Force, { pos })
  , m_inner_r(23.0f)
{
  const auto t = power;
  m_health = static_cast<int32_t>(std::max(fw::lerp(1.f, 50.f, t), 10.f));
  m_circle.setRadius(m_inner_r);
  m_circle.setOrigin(-m_pos.x+m_inner_r,-m_pos.y+ m_inner_r);
  m_circle.setFillColor(sf::Color(0));
  m_circle.setOutlineThickness(0.3f);
  m_circle.setOutlineColor(const_param::ticcol(6));
  m_colli_attr.on(colli_attr);
}

void ForceF::init()
{
  this->attr_forces();
  this->set_radius(29.0f);
  FW_ASSERT(m_radius >= m_inner_r);
}

void ForceF::update(float dt)
{
  m_elp += dt;
  if (m_elp >= 0.4f) {
    this->del();
  }
}

void ForceF::draw(sf::RenderWindow& window)
{
  window.draw(m_circle);
}

void ForceF::hitcb_w(Entity* o, const Vec2f& dir, float d) const
{
  if (d < o->get_radius() + m_inner_r) {
    o->on_hit_mask(m_colli_attr);
    o->sub_health_dmg(static_cast<int32_t>(m_health * o->get_exp_resi()));
  } else {
    o->add_vel_force(dir / d * 2.f);
  }
}


ForceD::ForceD(const Vec2f pos)
  : Entity(EntityType::Force, { pos })
{
}

void ForceD::init()
{
  this->attr_forces();
}

void ForceD::update(float dt)
{
}
