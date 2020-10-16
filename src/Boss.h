#pragma once
#include "Enemy.h"
namespace scr {
class ILuaScript;
}

class Boss : public Enemy {
public:
  Boss(const EntityArgs& args, uint32_t spr_ene=0);
  virtual ~Boss() = default;
protected:
  int32_t m_health_max=1;
};

class BossBaby final : public Boss {
public:
  BossBaby(const EntityArgs& args);
  virtual ~BossBaby() = default;
  void appear() override;
  void update(float dt) override;
  void upd_ene(float dt) override;
  void draw(sf::RenderWindow& window) override;
private:
  void arms0(float t, int32_t num=10, float ofs=0.0f);

  scr::ILuaScript* m_script=nullptr;
  sf::CircleShape m_circle;
  sf::Sprite m_spr_body;
  float m_arms_timer = 0.0f;
};