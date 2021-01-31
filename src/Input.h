#pragma once
#include <optional>
#include "Singleton.h"
#include "Vec2.h"
#include "ConstParam.h"

enum InputButton : uint32_t {
  InputButton_Up = 1 << 0,
  InputButton_Down = 1 << 1,
  InputButton_Left = 1 << 2,
  InputButton_Right = 1 << 3,
  InputButton_Shot = 1 << 4,
  InputButton_Dash = 1 << 5,
  InputButton_PadDash = 1 << 6,

  InputButton_UseKeybord = 1 << 14, //keybord使用
  InputButton_UseJoystick = 1 << 15, //joystic使用
  InputButton_Decide = 1 << 16,
  InputButton_Cancel = 1 << 17,
};

struct InputData {
  uint32_t m_mask = 0;
  uint32_t m_mask_trig = 0;
  uint32_t m_mask_off = 0;
  Vec2f m_mpos;
  Vec2f m_analog_l, m_analog_r;
  void reset() {
    m_mask = 0;
    m_mask_trig = 0;
    m_mask_off = 0;
    m_mpos.set(0.0f, 0.0f);
    m_analog_l.set(0.0f, 0.0f);
    m_analog_r.set(0.0f, 0.0f);
  }
  bool on(InputButton b) const { return (m_mask & b) != 0; }
  bool trig(InputButton b) const { return (m_mask_trig & b) != 0; }
  bool off(InputButton b) const { return (m_mask_off & b) != 0; }
  const Vec2f& mouse() const { return m_mpos; }
};
struct InputPlayer {
  bool m_enable_keybord = false;
  std::optional<uint32_t> m_joystick_id;
  bool m_use_joystick = false; //keybord or joystick
  void reset() {
    m_enable_keybord = false;
    m_joystick_id.reset();
  }
};

union InputKeyData {
  InputKeyData()
    : all(0)
  {
    m_click_count = std::numeric_limits<decltype(m_click_count)>::max();
  }
  struct {
    bool m_on : 1;
    bool m_trig : 1;
    bool m_off : 1;
    bool m_repeat : 1;
    bool m_already_repeat : 1;
    bool m_dclick : 1;
    uint16_t m_repeat_count;
    uint8_t m_click_count;
  };
  uint32_t all = 0;
  void update(const bool in_flag);
};
//static_assert(sizeof(InputKeyData) == 4);

class Input : public Singleton<Input> {
public:
  Input() = default;
  ~Input() = default;
  void update_system();
  float update(float dt, sf::RenderWindow& window);
  void update_assignment(const uint32_t player_num, bool force_flag=false);
  const auto& input_pair(uint32_t id) const { FW_ASSERT(id < m_input_data.size()); return m_input_data[id]; }
  const InputData& input_data(uint32_t id) const { FW_ASSERT(id<m_input_data.size()); return m_input_data[id].second; }
  bool decided() const;
  bool canceled() const;
#if DEBUG
  bool dbg_pause() const { return m_pause_key.m_repeat; }
  bool dbg_pause_cancel() const { return m_pause_cancel.m_trig; }
  bool dbg_escape() const { return m_esc_key.m_dclick; }
  bool dbg_reset() const { return m_reset_key.m_trig; }
#endif
private:
  static uint32_t update_keyborad();
  static Vec2f update_mouse(sf::RenderWindow& window);
  static uint32_t update_joystick(uint32_t id, Vec2f& analog_l, Vec2f& analog_r);
  //system
  InputKeyData m_pause_key;
  InputKeyData m_pause_cancel;
  InputKeyData m_esc_key;
  InputKeyData m_reset_key;
  //current
  std::array<std::pair<InputPlayer,InputData>, const_param::PLAYER_NUM_MAX> m_input_data;
  float m_dt = 0.0f;
  uint32_t m_joystick_num = UINT32_MAX;
  Vec2f m_prev_mxy;
};