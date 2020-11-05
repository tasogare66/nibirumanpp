#pragma once
#include "Mode.h"
class ModeTitle : public Mode {
public:
  ModeTitle(ModeType in_mode_type);
  ~ModeTitle();
  void init() override;
  void dest() override;
  bool ctrl(float dt) override;
  void ctrl_post() override;
  void draw0(sf::RenderWindow& window) override;
  void draw1(sf::RenderWindow& window) override;
  void draw2(sf::RenderWindow& window) override;
private:
  float m_elp = 0.0f;
  bool m_decided = false;
  float m_decided_time = 0.0f;
  sf::Text m_text;
  static int32_t sCURSOR;
  std::vector<sf::Vector3i> m_p;
  sf::Sprite m_spr;
};