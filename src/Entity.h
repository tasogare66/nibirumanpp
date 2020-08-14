#pragma once
#include "Vec2.h"
#include "FwFlag.h"
class Shash;

enum class EntityType : uint32_t {
  None=0,
  Player,
  PlBullet,
  Enemy,
};
struct EntityArgs {
  EntityType m_type = EntityType::None;
  Vec2f m_pos;
  Vec2f m_dir{ 1.f,0.f };
  float m_radius = 3; //default
  float m_mass = 1; //default
  EntityArgs() = default;
  EntityArgs(EntityType type, const Vec2f& pos, const Vec2f& dir = { 1.f, 0.f })
    : m_type(type)
    , m_pos(pos)
    , m_dir(dir)
  {}
  Vec2f aabb0() const { return m_pos - Vec2f(m_radius,m_radius); }
};

enum class EntityFlag : uint32_t {
  del = 1 << 0,
  AttrVerlet = 1 << 1,
  Ally = 1 << 2,
  Invincible = 1 << 3,
};
enum class HitMask : uint32_t {
  Enemy = 1 << 0,
  Player = 1 << 1,
};

class Entity {
public:
  explicit Entity(const EntityArgs& args);
  virtual ~Entity();
  void attr_px();
  void attr_bullet();
  void attr_ene_bullet();
  void attr_ene_dot();
  virtual void init() {}
  virtual void update(float dt);
  virtual void draw(sf::RenderWindow& window);
  virtual bool hit_wall(const Vec2f&) { return false; } //true‚Ìê‡repulse‚È‚µ
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
  //exp_resi = 1,
  FwFlag<HitMask> m_hit_mask; //HitMask
  FwFlag<HitMask> m_colli_attr; //hitŽž’Ê’m‚·‚éattribute
  sf::Sprite m_spr;
};