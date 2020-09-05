#pragma once
#include "Enemy.h"

class EneGrunt : public Enemy {
public:
  EneGrunt(const EntityArgs& args);
  ~EneGrunt() = default;
private:
  void appear() override final;
  void upd_ene(float dt) override final;
  float m_rad = 0.f;
};