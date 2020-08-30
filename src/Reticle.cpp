#include "stdafx.h"

#include "Input.h"
#include "Reticle.h"

Reticle::Reticle(const EntityArgs& args)
  : Entity(EntityType::None, args)
{
}

void Reticle::update(float dt)
{
  m_pos = Input::inst().mouse();
}
