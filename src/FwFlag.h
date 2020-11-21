#pragma once
template<typename T>
class FwFlag {
public:
  using underlying_type_t = typename std::underlying_type<T>::type;
  FwFlag() = default;
  FwFlag(underlying_type_t in_mask)
    : m_mask(in_mask)
  {}
  ~FwFlag() = default;

  void set(T m) {
    m_mask |= static_cast<decltype(m_mask)>(m);
  }
  void set(FwFlag<T> m) {
    m_mask |= m.m_mask;
  }
  //void set(FwFlag<T> m) {
  //  m_mask = m;
  //}
  void set(T m, bool flag) {
    if (flag) {
      this->set(m);
    } else {
      this->reset(m);
    }
  }
  void reset(T m) {
    m_mask &= ~static_cast<decltype(m_mask)>(m);
  }
  //全部クリア
  void reset() {
    m_mask = 0;
  }
  bool test(T m) const {
    return ((m_mask & static_cast<decltype(m_mask)>(m)) != 0);
  }
  //いずれか1だとtrue
  bool any() const {
    return (m_mask != 0);
  }
  //すべて0だとtrue
  bool none() const {
    return (m_mask == 0);
  }
  underlying_type_t value() const {
    return m_mask;
  }
protected:
  underlying_type_t m_mask = 0;
};