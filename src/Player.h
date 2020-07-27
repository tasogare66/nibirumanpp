#pragma once
#include "Entity.h"

class Player : public Entity {
public:
  Player();
  virtual ~Player() = default;
  void update(float dt) override;
private:
};