#pragma once
#include "ModeType.h"
class Mode {
public:
  Mode(ModeType in_mode_type)
    : m_mode_type(in_mode_type)
  {}
  virtual ~Mode() = default;
  virtual void init() = 0;
  virtual void dest() = 0;
  virtual bool ctrl(float dt) = 0;
  virtual void ctrl_post() = 0;
  virtual void draw0(sf::RenderWindow& window) = 0;
  virtual void draw1(sf::RenderWindow& window) = 0;
  virtual void draw2(sf::RenderWindow& window) = 0;

  ModeType get_mode_type() const { return m_mode_type; }
private:
  ModeType m_mode_type = ModeType::NONE;
};