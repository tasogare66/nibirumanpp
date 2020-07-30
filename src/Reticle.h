#pragma once
#include "Entity.h"

class Reticle : public Entity {
public:
  Reticle();
  virtual ~Reticle() = default;
  void update(float dt) override;
private:
};