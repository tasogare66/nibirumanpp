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
    Vec2f al, ar;
    //[-100 .. 100]
    al.x += sf::Joystick::getAxisPosition(jsid, sf::Joystick::X)/100.0f;
    al.y += sf::Joystick::getAxisPosition(jsid, sf::Joystick::Y)/100.0f;
    al.x += sf::Joystick::getAxisPosition(jsid, sf::Joystick::PovX)/100.0f;
    al.y -= sf::Joystick::getAxisPosition(jsid, sf::Joystick::PovY)/100.0f;
    ar.x += sf::Joystick::getAxisPosition(jsid, sf::Joystick::U)/100.0f;
    ar.y += sf::Joystick::getAxisPosition(jsid, sf::Joystick::V)/100.0f;
    constexpr float threshold = 0.4f;
    if (al.y < -threshold) {
      m |= InputButton_Up;
    }
    if (al.y > threshold) {
      m |= InputButton_Down;
    }
    if (al.x < -threshold) {
      m |= InputButton_Left;
    }
    if (al.x > threshold) {
      m |= InputButton_Right;
    }
    //A:1
    //B:2
    //LB:4,RB:5
    //BACK:6,START:7
    if (sf::Joystick::isButtonPressed(jsid, 0)) {
      m |= InputButton_Decide;
    }
    if (sf::Joystick::isButtonPressed(jsid, 1)) {
      m |= InputButton_Cancel;
    }
    if (sf::Joystick::isButtonPressed(jsid, 4)) {
      m |= InputButton_PadDash;
    }
    if (sf::Joystick::getAxisPosition(jsid, sf::Joystick::Z)/100.0f > threshold) {
      m |= InputButton_PadDash;
    }
    //static int test = 0;
    //if (sf::Joystick::isButtonPressed(jsid, test)) {
    //  m |= InputButton_Dash;
    //}

    if (m || al.sqr_magnitude() > 0.25f || ar.sqr_magnitude() > 0.25f) {
      m |= InputButton_UseJoystick;
    }
    analog_l += al;
    analog_r += ar;
  }
  return m;
}

void InputKeyData::update(const bool in_flag)
{
  m_trig = !m_on && in_flag;
  m_repeat = m_trig; //repeat |= trig
  m_off = m_on && !in_flag;
  m_on = in_flag;

  //repeat
  if (m_on) {
    ++m_repeat_count;
  } else {
    m_repeat_count = 0;
    m_already_repeat = false;
  }
  if (m_repeat_count > (m_already_repeat ? 4 : 40)) {
    m_repeat = true;
    m_already_repeat = true;
    m_repeat_count = 0;
  }

  //double click
  m_dclick = false;
  auto nxt = m_click_count + 1;
  if (m_click_count < nxt) m_click_count = nxt;
  if (m_trig) {
    if (m_click_count < 15) {
      m_dclick = true;
    }
    m_click_count = 0;
  }
}
void Input::update_system()
{
  auto lambda_update_key = [](InputKeyData& kd, sf::Keyboard::Key in_key) {
    bool flag = sf::Keyboard::isKeyPressed(in_key);
    kd.update(flag);
  };
  lambda_update_key(m_pause_key, sf::Keyboard::P);
  lambda_update_key(m_pause_cancel, sf::Keyboard::O);
  lambda_update_key(m_esc_key, sf::Keyboard::Escape);
}

float Input::update(float dt, sf::RenderWindow& window)
{
  for (uint32_t i=0;i<m_input_data.size();++i){
    auto& p = m_input_data[i].first;
    auto& d = m_input_data[i].second;
    uint32_t m = 0;
    Vec2f mxy;
    Vec2f analog_l, analog_r;
    if (p.m_enable_keybord) {
      m |= Input::update_keyborad();
      mxy = Input::update_mouse(window);
      //keyboard to analog
      if (m & InputButton_Up) analog_l.y -= 1.0f;
      if (m & InputButton_Down) analog_l.y += 1.0f;
      if (m & InputButton_Left) analog_l.x -= 1.0f;
      if (m & InputButton_Right) analog_l.x += 1.0f;

      //keybord使用判定
      if (m || (m_prev_mxy-mxy).sqr_magnitude() > 25.0f) {
        m |= InputButton_UseKeybord;
      }
      m_prev_mxy = mxy;
    }
    if (p.m_joystick_id) {
      m |= Input::update_joystick(p.m_joystick_id.value(), analog_l, analog_r);
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

    //keybord or joystick
    if (p.m_use_joystick) {
      if (m & InputButton_UseKeybord) p.m_use_joystick = false;
    }else{
      if (m & InputButton_UseJoystick) p.m_use_joystick = true;
    }
  }

  m_dt = dt;

  return dt;
}

void Input::update_assignment(const uint32_t player_num, bool force_flag)
{
  FW_ASSERT(player_num > 0 && player_num <= const_param::PLAYER_NUM_MAX);
  std::array<uint32_t, sf::Joystick::Count> connect;
  uint32_t num = 0;
  for (uint32_t i = 0; i < sf::Joystick::Count; ++i) {
    if (sf::Joystick::isConnected(i)) {
      connect[i] = i;
      ++num;
    }
  }

  if (!force_flag && m_joystick_num == num) return; //数が変わらなければやらない

  m_joystick_num = num;
  //clear
  for (auto& p:m_input_data) {
    auto& ply = p.first;
    auto& dat = p.second;
    ply.reset();
    dat.reset();
  }
  //0は必ずkeybord有効
  m_input_data[0].first.m_enable_keybord = true;
  //joystick割り当て
  uint32_t st = (num >= player_num) ? 0 : 1;
  for (uint32_t jid = 0; jid < num; ++jid) {
    auto pt = st + jid;
    if (pt>=m_input_data.size()) break;
    m_input_data[pt].first.m_joystick_id = jid;
  }
}

bool Input::decided() const
{
  return this->input_data(0).off(InputButton_Decide);
}

bool Input::canceled() const
{
  return this->input_data(0).off(InputButton_Cancel);
}
