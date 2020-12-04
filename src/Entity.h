#pragma once
#include <functional>
#include "Vec2.h"
#include "FwFlag.h"
class Shash;
class Player;
namespace fabrik {
  class Effector;
}

enum class EntityType : uint32_t {
  None=0,
  Player,
  PlBullet,
  Enemy,
  Dot,
  Human,
  Force,
};
struct EntityArgs {
  Vec2f m_pos;
  Vec2f m_dir{ 1.f,0.f };
  float m_radius = 3; //default
  float m_mass = 1; //default
  float m_param0 = 0.0f; //misc param
  EntityArgs() = default;
  EntityArgs(const Vec2f& pos, const Vec2f& dir = { 1.f, 0.f })
    : m_pos(pos)
    , m_dir(dir)
  {}
  Vec2f aabb0() const { return m_pos - Vec2f(m_radius,m_radius); }
};

enum class EntityFlag : uint32_t {
  Del = 1 << 0,
  Suicide = 1 << 1,
  AttrVerlet = 1 << 2,
  Ally = 1 << 3,
  //有効,無効
  UpdateEnabled = 1 << 4,
  ForceAddVelEnabled = 1 << 5, //forceで力加える
  Visible = 1 << 6,
  CollisionEnabled = 1 << 7,
  IgnoreCollisionSameRoot = 1 << 8, //同rootだとcollisionとらない

  Invincible = 1 << 9, //無敵,CollisionEnabledと併用
  HaveDot = 1 << 10,
  Spawned = 1 << 11,

  DefaultMask = (CollisionEnabled | UpdateEnabled | ForceAddVelEnabled | Visible), //defaultの設定値
};
enum class HitMask : uint32_t {
  Enemy = 1 << 0,
  Player0 = 1 << 1,
  Player1 = 1 << 2,
  Player2 = 1 << 3,
  Player3 = 1 << 4,
  Generated_Pl0 = 1 << 5,
  Generated_Pl1 = 1 << 6,
  Generated_Pl2 = 1 << 7,
  Generated_Pl3 = 1 << 8,

  PlayerAll = (Player0|Player1|Player2|Player3),
  Generated_PlAll = (Generated_Pl0| Generated_Pl1| Generated_Pl2| Generated_Pl3),
};
static constexpr int32_t Entity_PLAYER_MAX = 4;

class Entity {
public:
  explicit Entity(EntityType type, const EntityArgs& args);
  virtual ~Entity();
  void attr_px();
  void attr_bullet();
  void attr_ene_bullet();
  void attr_ene_dot();
  void attr_forces();
  void attr_spawned();
  virtual void init() {}
  virtual void update(float dt);
  virtual void draw(sf::RenderWindow& window);
  virtual bool hit_wall(const Vec2f&) { return false; } //trueの場合repulse行わない
  virtual void hitcb(const Entity*, const Vec2f&, float) {}
  virtual void hitcb_w(Entity*, const Vec2f&, float) const {}
  virtual void dead() {}
  virtual void set_blink() {}

  void set_radius(float ir);
  void set_mass(float imass);
  void set_position(const Vec2f& ipos);
  Vec2f get_pos() const { return m_pos; }
  Vec2f get_estimate_pos() const { return m_pos + m_mov; }
  void updateEstimateAABB();
  void set_dir(const Vec2f& idir) { m_dir = idir; }
  Vec2f get_dir() const { return m_dir; }
  Vec2f calc_mov(const Vec2f& pos) const { return (pos-m_pos); }
  void add_mov(const Vec2f& v) { m_mov += v; }
  void set_mov(const Vec2f& v) { m_mov = v; }
  void add_vel_force(const Vec2f& v) {
    m_old_pos -= v;
  }
  void set_vel_force(const Vec2f& v) {
    m_old_pos = m_pos - v;
  }
  void lim_vel_force(float l);
  void repulse(const Vec2f& inml, float dist);
  void pre_pro();
  void do_verlet(float dt, float inv_prev_dt, float decel);

  float get_radius() const { return m_radius; }
  const Vec2f& get_aabb0() const { return m_aabb0; }

  const FwFlag<EntityFlag>& get_flag() const { return m_flag; }
  const FwFlag<HitMask>& get_colli_attr() const { return  m_colli_attr; }
  void on_hit_mask(FwFlag<HitMask> mask) { m_hit_mask.set(mask); }

  void sub_health_dmg(int32_t dmg);
  void sub_health(const Entity* t);
  float get_exp_resi() const { return m_exp_resi; }

  bool check_kill_by_generated_player(std::function<void(int32_t)> cb) const;
  bool check_kill_by_player(std::function<void(int32_t)> cb) const;
  Player* check_kill_by_player_random() const;
  bool is_visible() const { return m_flag.test(EntityFlag::Visible); }
  void set_visible(bool in_flag) { m_flag.set(EntityFlag::Visible, in_flag); }
  void set_invincible(bool in_flag) { //無敵on/off
    m_flag.set(EntityFlag::Invincible, in_flag);
    m_flag.set(EntityFlag::CollisionEnabled, not in_flag);
  }

  template<typename T>
  const T* cast_to() const;

  //親子関係の設定
  static void set_hierarchy(Entity* parent, Entity* child);
  size_t get_child_count() const { return m_children.size(); }
  Entity* get_child_w(size_t idx) {
    if (idx < m_children.size()) return m_children[idx];
    FW_ASSERT(0);
    return nullptr;
  }
  const auto& get_children() const { return m_children; }
  auto& get_children_w() { return m_children; }
  fabrik::Effector* fetch_effector();
  void exec_lower(std::function<void(Entity*)> func); //dfs
  void exec_or_lower(std::function<void(Entity*)> func); //自分も実行

protected:
  friend class ObjLst;

  void set_sha(Shash* set_lst);
  void attr_verlet();
  Vec2f calc_velocity() const {
    return m_pos - m_old_pos;
  }
  void  set_velocity(const Vec2f& ivel) {
    m_vel = ivel;
  }
  void del() { m_flag.set(EntityFlag::Del); }
  void suiside() {
    constexpr auto flg = fw::underlying_cast(EntityFlag::Del) | fw::underlying_cast(EntityFlag::Suicide);
    m_flag.set(static_cast<EntityFlag>(flg));
  }
  void spr8x8(uint32_t id, uint16_t w=1, uint16_t h=1);

  EntityType m_type=EntityType::None;
  Vec2f m_pos;
  Vec2f m_dir; //向き
  Vec2f m_old_pos;
  Vec2f m_vel;
  float m_radius;
  Vec2f m_force;
  Vec2f m_mov;
  Vec2f m_mov_old;
  float m_mass, m_inv_mass;
  Vec2f m_aabb0;
  Vec2f m_aabb_size;
  Vec2f m_half_extents;
  Shash* m_sha = nullptr;
  FwFlag<EntityFlag> m_flag;
  uint32_t m_no = 0;
  int32_t m_health = 1;
  float m_exp_resi = 1.0f;
  uint32_t m_score = 0;
  FwFlag<HitMask> m_hit_mask; //HitMask
  FwFlag<HitMask> m_colli_attr; //hit時通知するattribute
  sf::Sprite m_spr;
  static constexpr uint32_t m_dummy_spr_id = 1;
  uint32_t m_spr_id = m_dummy_spr_id;
  Entity* m_root = nullptr;
  Entity* m_parent = nullptr;
  std::vector<Entity*> m_children;
  std::unique_ptr<fabrik::Effector> m_effector;
};