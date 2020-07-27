#pragma once
class Mode {
public:
  virtual ~Mode() = default;
  virtual void init() = 0;
  virtual void dest() = 0;
  virtual bool ctrl(float dt) = 0;
  virtual void ctrl_post() = 0;
  virtual void draw0(sf::RenderWindow& window) = 0;
  virtual void draw1(sf::RenderWindow& window) = 0;
};