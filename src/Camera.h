#pragma once
#include "Singleton.h"
#include "Vec2.h"
class Player;

class Camera : public Singleton<Camera> {
public:
  Camera() = default;
  ~Camera() = default;
  void upd(const std::vector<Player*>&);
  const sf::View& update_view();
  const sf::View& get_view() const { return m_view; }
  const sf::View& get_view_2d() const { return m_view_2d; }
  Vec2f get_top_left() const { return m_view.getCenter() - m_view.getSize() / 2.0f; }
  void req_shake(float v) { m_shake = std::min(m_shake + v, 4.0f); }
  void reset() {
    m_inr_center.set(0.0f, 0.0f);
    m_center.set(0.0f, 0.0f);
    m_shake = 0.0f;
  }
private:
  Vec2f upd_shake();
  Vec2f m_inr_center;
  Vec2f m_center;
  Vec2f m_top_left;
  Vec2f m_trs;
  float m_scale = 4.0;
  sf::View m_view;
  sf::View m_view_2d;
  float m_shake = 0.0f;
};