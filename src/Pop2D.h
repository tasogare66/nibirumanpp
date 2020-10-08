#pragma once
#include "Entity.h"

class Pop2D : public Entity {
public:
  explicit Pop2D(const EntityArgs& args);
  virtual ~Pop2D() = default;
  void update(float dt) override;
private:
  float m_elp = 0.0f;
  Vec2f m_pos_st;
  uint32_t m_spr_st=484;
};