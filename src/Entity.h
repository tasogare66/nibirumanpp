#pragma once
#include "Vec2.h"
#include "FwFlag.h"
class Shash;

enum class EntityType : uint32_t {
  None=0,
  Player,
  PlBullet,
  Enemy,
  Dot,
  Human,
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
  del = 1 << 0,
  AttrVerlet = 1 << 1,
  Ally = 1 << 2,
  Invincible = 1 << 3,
  HaveDot = 1 << 4,
  Spawned = 1 << 5,
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
};

class Entity {
public:
  explicit Entity(EntityType type, const EntityArgs& args);
  virtual ~Entity();
  void attr_px();
  void attr_bullet();
  void attr_ene_bullet();
  void attr_ene_dot();
  void attr_spawned();
  virtual void init() {}
  virtual void update(float dt);
  virtual void draw(sf::RenderWindow& window);
  virtual bool hit_wall(const Vec2f&) { return false; } //true�̏ꍇrepulse�Ȃ�
  virtual void hitcb(const Entity*, const Vec2f&, float) {}
  virtual void dead() {}
  virtual void set_blink() {}

  void set_radius(float ir);
  void set_mass(float imass);
  void set_position(const Vec2f& ipos);
  Vec2f get_pos() const { return m_pos; }
  Vec2f get_estimate_pos() const { return m_pos + m_mov; }
  void updateEstimateAABB();
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

  void sub_health_dmg(int32_t dmg);
  void sub_health(const Entity* t);

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
  void del() { m_flag.on(EntityFlag::del); }
  void spr8x8(uint32_t id);

  EntityType m_type=EntityType::None;
  Vec2f m_pos;
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
  int32_t m_exp_resi = 1;
  uint32_t m_score = 0;
  FwFlag<HitMask> m_hit_mask; //HitMask
  FwFlag<HitMask> m_colli_attr; //hit時通知するattribute
  sf::Sprite m_spr;
  static constexpr uint32_t m_dummy_spr_id = 1;
  uint32_t m_spr_id = m_dummy_spr_id;
};