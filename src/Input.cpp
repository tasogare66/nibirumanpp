#include "stdafx.h"

#include "Camera.h"
#include "Input.h"

uint32_t Input::update_keyborad()
{
  uint32_t m = 0;
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

  return m;
}

Vec2f Input::update_mouse(sf::RenderWindow& window)
{
  //mouse position
  auto imousePos = sf::Mouse::getPosition(window); //window position
  auto mxy = window.mapPixelToCoords(imousePos, Camera::inst().get_view()); //window position to global position
  return mxy;
}

uint32_t Input::update_joystick(uint32_t jsid, Vec2f& analog_l, Vec2f& analog_r)
{
  uint32_t m = 0;
  if (sf::Joystick::isConnected(jsid)) {
    //[-100 .. 100]
    analog_l.x += sf::Joystick::getAxisPosition(jsid, sf::Joystick::X)/100.0f;
    analog_l.y += sf::Joystick::getAxisPosition(jsid, sf::Joystick::Y)/100.0f;
    analog_r.x += sf::Joystick::getAxisPosition(jsid, sf::Joystick::U)/100.0f;
    analog_r.y += sf::Joystick::getAxisPosition(jsid, sf::Joystick::V)/100.0f;
    constexpr float threshold = 0.4f;
    if (analog_l.y < -threshold) {
      m |= InputButton_Up;
    }
    if (analog_l.y > threshold) {
      m |= InputButton_Down;
    }
    if (analog_l.x < -threshold) {
      m |= InputButton_Left;
    }
    if (analog_l.x > threshold) {
      m |= InputButton_Right;
    }
    if (sf::Joystick::isButtonPressed(jsid, 0)) {
      m |= InputButton_Decide;
    }
    if (sf::Joystick::isButtonPressed(jsid, 1)) {
      m |= InputButton_Cancel;
    }
    //RB:5
    //static int test = 0;
    //if (sf::Joystick::isButtonPressed(jsid, test)) {
    //  m |= InputButton_Dash;
    //}
  }
  return m;
}

float Input::update(float dt, sf::RenderWindow& window)
{
#if 01
  for (uint32_t i=0;i<m_input_data.size();++i){
    auto& d = m_input_data[i];
    uint32_t m = 0;
    Vec2f mxy;
    Vec2f analog_l, analog_r;
    if (true) {
      m |= Input::update_keyborad();
      mxy = Input::update_mouse(window);
      //keyboard to analog
      if (m & InputButton_Up) analog_l.y -= 1.0f;
      if (m & InputButton_Down) analog_l.y += 1.0f;
      if (m & InputButton_Left) analog_l.x -= 1.0f;
      if (m & InputButton_Right) analog_l.x += 1.0f;
    }
    if (true) {
      m |= Input::update_joystick(i, analog_l, analog_r);
    }
    //clamp
    auto apply_clamp = [](auto& v) {
      v = std::clamp(v, -1.0f, 1.0f);
    };
    apply_clamp(analog_l.x);
    apply_clamp(analog_l.y);
    apply_clamp(analog_r.x);
    apply_clamp(analog_r.y);

    d.m_mask_trig = m & (~d.m_mask);
    d.m_mask_off = d.m_mask & (~m);
    d.m_mask = m;
    d.m_mpos = mxy;
    d.m_analog_l = analog_l;
    d.m_analog_r = analog_r;
  }
#else
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
#endif
  m_dt = dt;

  return dt;
}

bool Input::decided() const
{
  return this->input_data(0).off(InputButton_Decide);
}

bool Input::canceled() const
{
  return this->input_data(0).off(InputButton_Cancel);
}
