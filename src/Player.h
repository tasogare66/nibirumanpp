#pragma once
#include "Entity.h"

class Player : public Entity {
public:
  explicit Player(const EntityArgs& args);
  virtual ~Player() = default;
  void init() override;
  void update(float dt) override;
private:
  bool is_dashing() const { return false; }
  int32_t m_shot_repeat = 0;
  float m_invincible_time = 0.f;
  int32_t m_armslv = 0;
};