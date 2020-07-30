#include "stdafx.h"

#include "Input.h"

float Input::update(float dt, sf::RenderWindow& window)
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
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    m |= InputButton_Shot;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
    m |= InputButton_Dash;
  }
  sf::Vector2f mxy(sf::Mouse::getPosition(window));

  m_mask = m;
  m_mpos.set(mxy.x, mxy.y);
  m_dt = dt;

  return dt;
}
