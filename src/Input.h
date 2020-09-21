#pragma once
#include "Singleton.h"
#include "Vec2.h"

enum InputButton : uint32_t {
  InputButton_Up = 1 << 0,
  InputButton_Down = 1 << 1,
  InputButton_Left = 1 << 2,
  InputButton_Right = 1 << 3,
  InputButton_Shot = 1 << 4,
  InputButton_Dash = 1 << 5,

  InputButton_Decide = 1 << 6,
  InputButton_Cancel = 1 << 7,
};

class Input : public Singleton<Input> {
public:
  Input() = default;
  ~Input() = default;
  float update(float dt, sf::RenderWindow& window);
  bool on(InputButton b) const { return (m_mask & b) != 0; }
  bool trig(InputButton b) const { return (m_mask_trig & b) != 0; }
  bool off(InputButton b) const { return (m_mask_off & b) != 0; }
  const Vec2f& mouse() const { return m_mpos; }
  bool decided() const;
  bool canceled() const;
private:
  //current
  uint32_t m_mask = 0;
  uint32_t m_mask_trig = 0;
  uint32_t m_mask_off = 0;
  Vec2f m_mpos;
  float m_dt = 0.0f;
};