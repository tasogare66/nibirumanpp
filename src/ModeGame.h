#pragma once
#include "Mode.h"
#include "Spawner.h"
class ModeGame : public Mode {
public:
  ModeGame(ModeType in_mode_type);
  ~ModeGame();
  void init() override;
  void dest() override;
  bool ctrl(float dt) override;
  void ctrl_post() override;
  void draw0(sf::RenderWindow& window) override;
  void draw1(sf::RenderWindow& window) override;
  void draw2(sf::RenderWindow& window) override;
  auto& get_spawner_w() { return m_spawner; }
  const auto& get_spawner() const { return m_spawner; }
private:
  void base_clr();
  Spawner m_spawner;
  sf::CircleShape m_stg_circle;
  sf::Text m_score_text;
};