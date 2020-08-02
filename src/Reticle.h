#pragma once
#include "Entity.h"

class Reticle : public Entity {
public:
  explicit Reticle(const EntityArgs& args);
  virtual ~Reticle() = default;
  void update(float dt) override;
private:
};