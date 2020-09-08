#pragma once
#include "Entity.h"

class Player;
class Enemy :public Entity {
public:
  Enemy(const EntityArgs& args, uint32_t spr_ene=0);
  virtual ~Enemy() = default;
  virtual void update(float dt) override;
  virtual void dead() override;
protected:
  virtual void appear() {}
  virtual void upd_ene(float dt) = 0;
  bool m_appear_flag = true;
  float m_elapsed = 0.0f;
  uint32_t m_spr_ene = Entity::m_dummy_spr_id;
};

class EneSnake : public Enemy {
public:
  EneSnake(const EntityArgs& args);
  ~EneSnake() = default;
private:
  void appear() override final;
  void upd_ene(float dt) override final;
  const Player* m_tgt = nullptr;
};

class EneGrunt : public Enemy {
public:
  EneGrunt(const EntityArgs& args);
  ~EneGrunt() = default;
private:
  void appear() override final;
  void upd_ene(float dt) override final;
  float m_rad = 0.f;
};