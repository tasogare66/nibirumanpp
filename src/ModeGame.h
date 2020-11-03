#pragma once
#include "Mode.h"
#include "ConstParam.h"
#include "Spawner.h"
#include "Background.h"
class ModeGame : public Mode {
public:
  enum class State {
    Entry,
    Play,
    Over,
  };
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
  void upd_info(float dt);
  Spawner m_spawner;
  State m_state = State::Entry;
  float m_ovelp = 0.0f;
  float m_enttm = const_param::SCR_WIDTH + 10.f;
  float m_entstrx = 50.f;
  sf::CircleShape m_stg_circle;
  sf::Text m_text;
  sf::Sprite m_spr;
  Background m_bg;
};