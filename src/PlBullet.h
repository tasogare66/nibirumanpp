#pragma once
#include "Entity.h"
class Player;

class PlBullet : public Entity {
public:
  explicit PlBullet(const Vec2f pos, const Vec2f dir, const Player* owner);
  virtual ~PlBullet() = default;
  void init() override;
  void update(float dt) override;
  bool hit_wall(const Vec2f& dir) override;
private:
  float m_elp = 0.0f;
  Vec2f m_dir;
  float m_ang = 0.0f;
  const Player* m_owner = nullptr;
};