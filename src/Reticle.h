#pragma once
#include "Entity.h"

class Reticle : public Entity {
public:
  explicit Reticle(const EntityArgs& args, int32_t index);
  virtual ~Reticle() = default;
  void update(float dt) override;
private:
  const int32_t m_index; //plyaer index
};