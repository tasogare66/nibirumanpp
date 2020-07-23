#pragma once
class Entity {
public:
  Entity();
  virtual ~Entity();
  virtual void update();
  virtual void draw(sf::RenderWindow& window);
protected:
  sf::Sprite m_spr;
};