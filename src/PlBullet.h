#pragma once
#include "Entity.h"

class PlBullet : public Entity {
public:
  PlBullet();
  virtual ~PlBullet() = default;
  void update(float dt) override;
private:
  float m_elp = 0.0f;
  Vec2f m_dir;
};