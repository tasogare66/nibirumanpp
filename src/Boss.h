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
  bool is_damaged() const { return m_boss_flag.m_damaged; }
  void set_damaged_flag(bool in_flag) { m_boss_flag.m_damaged = in_flag; }
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
  } m_boss_flag;
};

class Boss : public BossParts {
public:
  Boss(const EntityArgs& args, uint32_t spr_ene=0);
  virtual ~Boss();
  virtual void update(float dt) override;

  virtual float move_to(float px, float py, float spd);
  virtual void use_arms(int type, const LuaIntf::LuaRef& tbl) {}
  virtual void set_stiffness(float,float) {}
  virtual void set_rot_speed(float, float) {}

  virtual void draw_info1(sf::RenderWindow&) const;
  virtual void draw_info2(sf::RenderWindow&, sf::Text& text) const;
  static bool is_exist_boss();
protected:
  void update_dt(float dt) { m_dt = dt; }
  void upd_ene_base(float dt);
  void dead_base();
  void set_bossrf();
  void remove_bossrf();
  Vec2f calc_dir(Vec2f tgt);
  int32_t m_health_max=1;
  float m_dt=1.0f/60.0f;
  float m_arms_timer = 0.0f;
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
  Vec2f m_dspofs;
  uint32_t m_animcnt=0;
};

class BossWorm final : public Boss {
public:
  BossWorm(const EntityArgs& args);
  virtual ~BossWorm() = default;
  void appear() override;
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
};

struct EaseParam {
public:
  EaseParam(float in_val)
    : m_current(in_val)
    , m_in(in_val)
    , m_out(in_val)
  {}
  virtual ~EaseParam() = default;
  void update(float dt) {
    m_elp = std::min(m_elp + dt, m_duration);
    if (m_elp >= m_duration) {
      m_current = m_out;
    } else {
      auto t = m_elp / m_duration;
      t = std::clamp(t, 0.f, 1.f);
      m_current = fw::lerp(m_in, m_out, ease_in_out_sin(t));
    }
  }
  void request(float req, float dur) {
    m_in = m_current;
    m_out = req;
    m_elp = 0.0f;
    m_duration = dur;
  }
  void reset() {
    m_in = m_current;
    m_out = m_current;
    m_duration = 0.0f;
    m_elp = 0.0f;
  }
  float get() const { return m_current; }
protected:
  static float ease_in_out_sin(float x){
    return -(std::cos(static_cast<float>(M_PI) * x) - 1.f) / 2.f;
  }
  float m_current;
  float m_in;
  float m_out;
  float m_duration=0.0f;
  float m_elp=0.0f;
};

class UrchinNode;
class BossUrchin final : public Boss {
public:
  class Node {
  public:
    Vec2f m_p;
    UrchinNode* m_e;
  };
  BossUrchin(const EntityArgs& args);
  virtual ~BossUrchin() = default;
  void appear() override;
  void update(float dt) override;
  void upd_ene(float dt) override;
  void draw(sf::RenderWindow& window) override;
  void set_stiffness(float v, float t) override { m_stiffness.request(v,t); }
  float get_stiffness() const { return m_stiffness.get(); }
  void set_rot_speed(float v, float t) override { m_rot_speed.request(v, t); }
  float get_rot_speed() const { return m_rot_speed.get(); } //degree

  void use_arms(int type, const LuaIntf::LuaRef& tbl) override;
private:
  void upd_nodes(bool is_reset=false);
  void arms0(float t); //末端から跳ね返り
  void arms1(float t); //中心から弾
  void arms2(float t); //敵生成
  static constexpr int32_t m_legs_num = 10;
  static constexpr int32_t m_node_num = 19;
  static constexpr float m_node_radisu = 4.0f;
  std::array<std::array<Node, m_node_num>, m_legs_num> m_all_nodes;
  float m_legs_rot = 0.0f;
  EaseParam m_stiffness{0.2f};
  EaseParam m_rot_speed{0.0f};
};