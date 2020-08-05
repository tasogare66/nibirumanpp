#pragma once
#include "Entity.h"

class PlBullet : public Entity {
public:
  explicit PlBullet(const Vec2f pos, const Vec2f dir);
  virtual ~PlBullet() = default;
  void init() override;
  void update(float dt) override;
private:
  float m_elp = 0.0f;
  Vec2f m_dir;
};