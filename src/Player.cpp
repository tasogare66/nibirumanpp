#include "stdafx.h"

#include "Input.h"
#include "Player.h"

Player::Player()
  : Entity()
{
  this->attr_px();
}

void Player::update(float dt)
{
  Vec2f v;
  if (Input::inst().on(InputButton_Up)) v.y -= 1.0f;
  if (Input::inst().on(InputButton_Down)) v.y += 1.0f;
  if (Input::inst().on(InputButton_Left)) v.x -= 1.0f;
  if (Input::inst().on(InputButton_Right)) v.x += 1.0f;
  if (v.sqr_magnitude() >= 0.01f) {
    v.normalize();
    m_mov = v * (60 * dt);
  }
}
