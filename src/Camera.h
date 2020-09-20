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
private:
  Vec2f upd_shake();
  Vec2f m_inr_center;
  Vec2f m_center;
  Vec2f m_top_left;
  Vec2f m_trs;
  float m_scale = 4.0;
  sf::View m_view;
  sf::View m_view_2d;
};