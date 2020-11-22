#include "stdafx.h"

#include "Input.h"
#include "GameSeq.h"
#include "Player.h"

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

  bool visible = not inputp.m_use_joystick;
  const auto* pl = GameSeq::inst().get_player_entity(m_index);
  if (!pl || pl->is_gone()) { //死んだら非表示
    visible = false;
  }
  this->set_visible(visible);
}
