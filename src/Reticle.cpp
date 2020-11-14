#include "stdafx.h"

#include "Input.h"
#include "Reticle.h"

Reticle::Reticle(const EntityArgs& args, int32_t index)
  : Entity(EntityType::None, args)
  , m_index(index)
{
  this->spr8x8(480);
}

void Reticle::update(float dt)
{
  const auto& [inputp, inputd] = Input::inst().input_pair(m_index);;
  m_pos = inputd.mouse();

  this->set_visible(not inputp.m_use_joystick);
}
