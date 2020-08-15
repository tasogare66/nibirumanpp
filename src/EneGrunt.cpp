#include "stdafx.h"

#include "GameSeq.h"
#include "EneGrunt.h"

EneGrunt::EneGrunt(const EntityArgs& args)
  : Enemy(args)
{
  m_flag.on(EntityFlag::HaveDot);
}

void EneGrunt::appear()
{
  this->attr_px();
}

void EneGrunt::upd_ene(float dt)
{
  m_rad += fw::deg2rad(120.f * dt);
  auto len = 0.24f * GameSeq::inst().getDifV(1.f, 2.5f);
  m_mov.x += std::cos(m_rad) * len;
  m_mov.y += std::sin(m_rad) * len;
  //local s = (self.elapsed//(FRAME2SEC*12))%4
  //  local animdir = self.mov.x > 0 and 0 or 1
  //  self.spr = self.spr_ene + s + animdir * 4
}
