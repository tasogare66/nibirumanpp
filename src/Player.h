#pragma once
#include "Entity.h"

class Player : public Entity {
public:
  explicit Player(const EntityArgs& args, const Entity* reticle);
  virtual ~Player() = default;
  void init() override;
  void update(float dt) override;

  float get_capradius() const { return m_capradius; }

private:
  bool is_dashing() const { return false; }
  int32_t m_shot_repeat = 0;
  float m_invincible_time = 0.f;
  float m_capradius = 20.0f;
  int32_t m_armslv = 0;
  const Entity* m_reticle = nullptr;
};