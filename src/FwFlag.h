#pragma once
template<typename T>
class FwFlag {
public:
  FwFlag() = default;
  ~FwFlag() = default;
  void on(T m) {
    m_mask |= static_cast<decltype(m_mask)>(m);
  }
  void off(T m) {
    m_mask &= ~static_cast<decltype(m_mask)>(m);
  }
  bool check(T m) const {
    return ((m_mask & static_cast<decltype(m_mask)>(m)) != 0);
  }
protected:
  uint32_t m_mask = 0;
};