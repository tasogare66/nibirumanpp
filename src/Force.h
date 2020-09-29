#pragma once
#include "Entity.h"

class ForceF : public Entity {
public:
  explicit ForceF(const Vec2f pos);
  virtual ~ForceF() = default;
  void init() override;
  void update(float dt) override;
  void draw(sf::RenderWindow& window) override;
private:
  float m_elp = 0.0f;
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
