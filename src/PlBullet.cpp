#include "stdafx.h"

#include "PlBullet.h"

PlBullet::PlBullet()
{
}

void PlBullet::update(float dt)
{
  m_elp += dt;
  if (m_elp >= 1.0f) {
    this->del();
    return;
  }
  m_mov = m_dir * dt;
}
