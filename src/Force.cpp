#include "stdafx.h"

#include "Force.h"

ForceF::ForceF(const Vec2f pos)
  : Entity(EntityType::Force, { pos })
{
}

void ForceF::init()
{
  this->attr_forces();
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
