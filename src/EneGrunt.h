#pragma once
#include "Enemy.h"

class EneGrunt : public Enemy {
public:
  EneGrunt(const EntityArgs& args);
  ~EneGrunt() = default;
private:
  void appear() override;
  void upd_ene(float dt) override;
  float m_rad = 0.f;
};