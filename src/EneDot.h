#pragma once
#include "Entity.h"

class EneDot : public Entity {
public:
  EneDot(const EntityArgs& args);
  void init() override;
  void update(float dt) override;
  void hitcb(const Entity*, const Vec2f&, float) override;
private:
  float m_timer = 6.0f;
  float m_captime = 0.0f;
  const Entity* m_capture_pl = nullptr;
  uint32_t m_ene_spr = 482;
};