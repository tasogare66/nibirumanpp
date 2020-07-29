#include "stdafx.h"

#include "Input.h"

float Input::update(float dt)
{
  decltype(m_mask) m=0;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    m |= InputButton_Up;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    m |= InputButton_Down;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    m |= InputButton_Left;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    m |= InputButton_Right;
  }
  m_mask = m;

  return dt;
}
