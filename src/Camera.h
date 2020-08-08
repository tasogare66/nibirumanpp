#pragma once
#include "Singleton.h"
class Player;

class Camera : public Singleton<Camera> {
public:
  Camera() = default;
  ~Camera() = default;
  void upd(const std::vector<Player*>&);
private:
  Vec2f upd_shake();
  Vec2f m_inr_center;
  Vec2f m_center;
  Vec2f m_top_left;
  Vec2f m_trs;
  float m_scale = 4.0;
};