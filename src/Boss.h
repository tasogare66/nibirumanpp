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
  void set_delay_del(float time=-1.0f);
  bool is_damaged() const { return m_flags.m_damaged; }
  void set_damaged_flag(bool in_flag) { m_flags.m_damaged = in_flag; }
protected:
  void dead_dot_base(int32_t num, float radius);
  void dead_efc_base();
  void set_circle_radius(float radius);
  void draw_circle(sf::RenderWindow& window);
  //遅らせてdel設定
  void upd_delay_del(float dt);
  void apply_delay_del();

  sf::CircleShape m_circle;
  std::optional<float> m_delay_del_time;
  union Flag {
    struct {
      bool m_damaged : 1;
    };
    uint32_t all = 0;
  } m_flags;
};

class Boss : public BossParts {
public:
  Boss(const EntityArgs& args, uint32_t spr_ene=0);
  virtual ~Boss();
  virtual void update(float dt) override;
  virtual float move_to(float px, float py, float spd);
  virtual void use_arms(int type, const LuaIntf::LuaRef& tbl) {}
  virtual void draw_info1(sf::RenderWindow&) const;
  virtual void draw_info2(sf::RenderWindow&, sf::Text& text) const;
  static bool is_exist_boss();
protected:
  void update_dt(float dt) { m_dt = dt; }
  void upd_ene_base(float dt);
  void dead_base();
  void set_bossrf();
  void remove_bossrf();
  Vec2f get_dir(Vec2f tgt);
  int32_t m_health_max=1;
  float m_dt=1.0f/60.0f;
  std::unique_ptr<scr::ILuaScript> m_script;
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
  void set_sub_dmg(bool is_del, int32_t dmg) override;
  void dead() override;

  void use_arms(int type, const LuaIntf::LuaRef& tbl) override;
private:
  static void create_bullet(const Vec2f& pos, const Vec2f& dir, bool is_arrow);
  void arms0(float t, bool is_arrow);

  fabrik::IK m_ik;
  float m_arms_timer = 0.0f;
};