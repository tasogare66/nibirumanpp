#include "stdafx.h"

#include "Camera.h"
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

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    m |= InputButton_Decide;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
    m |= InputButton_Cancel;
  }

  //mouse position
  auto imousePos = sf::Mouse::getPosition(window); //window position
  auto mxy = window.mapPixelToCoords(imousePos, Camera::inst().get_view()); //window position to global position

  m_mask_trig = m & (~m_mask);
  m_mask_off = m_mask & (~m) ;
  m_mask = m;
  m_mpos = mxy;
  m_dt = dt;

  return dt;
}

bool Input::decided() const
{
  return this->off(InputButton_Decide);
}

bool Input::canceled() const
{
  return this->off(InputButton_Cancel);
}
