#pragma once
#include "FABRIK.h"
#include "Enemy.h"
namespace LuaIntf {
class LuaRef;
}
namespace scr {
class ILuaScript;
}

class BossParts : public Enemy {
public:
  BossParts(const EntityArgs& args, uint32_t spr_ene = 0);
  virtual ~BossParts() = default;
protected:
  void set_circle_radius(float radius);
  void draw_circle(sf::RenderWindow& window);
  sf::CircleShape m_circle;
};

class Boss : public BossParts {
public:
  Boss(const EntityArgs& args, uint32_t spr_ene=0);
  virtual ~Boss();
  virtual void update(float dt) override;
  virtual void move_to(float px, float py, float spd);
  virtual void use_arms(int type, const LuaIntf::LuaRef& tbl) {}
  virtual void draw_info1(sf::RenderWindow&) const;
  virtual void draw_info2(sf::RenderWindow&, sf::Text& text) const;
  static bool is_exist_boss();
protected:
  void update_dt(float dt) { m_dt = dt; }
  void upd_ene_base(float dt) { m_elapsed += dt; }
  void dead_base();
  void set_bossrf();
  void remove_bossrf();
  Vec2f get_dir(Vec2f tgt);
  int32_t m_health_max=1;
  float m_dt=1.0f/60.0f;
};

class BossBaby final : public Boss {
public:
  BossBaby(const EntityArgs& args);
  virtual ~BossBaby() = default;
  void appear() override;
  void update(float dt) override;
  void upd_ene(float dt) override;
  void draw(sf::RenderWindow& window) override;
  void dead() override;

  void use_arms(int type, const LuaIntf::LuaRef& tbl) override;
private:
  void arms0(float t, int32_t num=10, float ofs=0.0f);

  scr::ILuaScript* m_script=nullptr;
  sf::Sprite m_spr_body;
  float m_arms_timer = 0.0f;
  Vec2f m_dspofs;
  uint32_t m_animcnt=0;
};

class BossWorm final : public Boss {
public:
  BossWorm(const EntityArgs& args);
  virtual ~BossWorm() = default;
  void appear();
  void update(float dt) override;
  void upd_ene(float dt) override;
  void draw(sf::RenderWindow& window) override;
  void dead() override;

  void use_arms(int type, const LuaIntf::LuaRef& tbl) override;
private:
  fabrik::IK m_ik;
};