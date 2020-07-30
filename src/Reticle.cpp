#include "stdafx.h"

#include "Input.h"
#include "Reticle.h"

Reticle::Reticle()
{
}

void Reticle::update(float dt)
{
  m_pos = Input::inst().mouse();
}
