#include "stdafx.h"

#include "ConstParam.h"

#include "Pop2D.h"

Pop2D::Pop2D(const EntityArgs& args)
  : Entity(EntityType::None, args)
  , m_pos_st(args.m_pos)
{
  this->spr8x8(m_spr_st,2,1);
}

void Pop2D::update(float dt)
{
  if (m_elp >= 1.0f) {
    this->del();
  }
  m_elp = std::min(m_elp + dt, 1.0f);
  m_pos.y = m_pos_st.y - m_elp * 14.0f;

  auto anim = static_cast<uint32_t>(m_elp / (const_param::FRAME2SEC * 4)) % 2;
  this->spr8x8(m_spr_st + anim * 2, 2, 1);
}
