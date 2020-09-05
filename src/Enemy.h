#pragma once
#include "Entity.h"

class Player;
class Enemy :public Entity {
public:
  Enemy(const EntityArgs& args);
  virtual ~Enemy() = default;
  virtual void update(float dt) override;
  virtual void dead() override;
protected:
  virtual void appear() {}
  virtual void upd_ene(float dt) = 0;
  bool m_appear_flag = true;
  float m_elapsed = 0.0f;
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