#pragma once
#include "Entity.h"

class ForceF : public Entity {
public:
  explicit ForceF(const Vec2f pos, const HitMask colli_attr, float power=0.0f);
  virtual ~ForceF() = default;
  void init() override;
  void update(float dt) override;
  void draw(sf::RenderWindow& window) override;
protected:
  void hitcb_w(Entity*, const Vec2f&, float) const override;
private:
  float m_elp = 0.0f;
  float m_inner_r;
  sf::CircleShape m_circle;
};

class ForceD : public Entity {
public:
  explicit ForceD(const Vec2f pos);
  virtual ~ForceD() = default;
  void init() override;
  void update(float dt) override;
private:
  float m_elp = 0.0f;
};
