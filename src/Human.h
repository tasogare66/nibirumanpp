#pragma once
#include "Enemy.h"

class Human : public Enemy {
public:
  explicit Human(const EntityArgs& args);
  virtual ~Human();
  void dead() override;
protected:
  void appear() override final;
  void upd_ene(float dt) override final;
  float m_timer = -1.0f;
  int32_t m_animdir = 2;
  Vec2f m_dir;
};