#pragma once
#include "Mode.h"
#include "ConstParam.h"
#include "Spawner.h"
#include "Background.h"
class Boss;
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
  bool is_exist_boss() const { return (m_boss != nullptr); }
  void clear_bossrf() { m_boss = nullptr; }
  void set_bossrf(const Boss* in_boss) { if(!m_boss) m_boss = in_boss; } //未登録ならいれる
  void remove_bossrf(const Boss* in_boss) { if (m_boss = in_boss) this->clear_bossrf(); }
private:
  void upd_info(float dt);
  Spawner m_spawner;
  State m_state = State::Entry;
  float m_ovelp = 0.0f;
  float m_enttm = const_param::SCR_WIDTH + 10.f;
  float m_entstrx = 50.f;
  sf::Text m_text;
  sf::Sprite m_spr;
  Background m_bg;
  const Boss* m_boss = nullptr;
};