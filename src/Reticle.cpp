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
  m_pos = Input::inst().input_data(m_index).mouse();
}
