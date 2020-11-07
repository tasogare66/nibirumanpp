#pragma once
#include "Entity.h"

class EneDot : public Entity {
public:
  EneDot(const EntityArgs& args);
  virtual ~EneDot();
  void init() override;
  void update(float dt) override;
  void hitcb(const Entity*, const Vec2f&, float) override;
  void dead() override;
private:
  float m_timer = 6.0f;
  float m_captime = 0.0f;
  const Player* m_capture_pl = nullptr;
  bool m_acquired = false; //playerが取った
  uint32_t m_ene_spr = 482;
};