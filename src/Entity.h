#pragma once
#include "Vec2.h"
#include "FwFlag.h"

enum class EntityFlag : uint32_t {
  del = 1 << 0,
  AttrVerlet = 1 << 1,
};

class Entity {
public:
  Entity();
  virtual ~Entity();
  void attr_px();
  void attr_bullet();
  void attr_ene_bullet();
  void attr_ene_dot();
  virtual void update(float dt);
  virtual void draw(sf::RenderWindow& window);

  void set_radius(float ir);
  void set_mass(float imass);
  void set_position(const Vec2f& ipos);
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

protected:
  void attr_verlet();
  Vec2f calc_velocity() const {
    return m_pos - m_old_pos;
  }
  void  set_velocity(const Vec2f& ivel) {
    m_vel = ivel;
  }

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
  //sha = nil,
  FwFlag<EntityFlag> m_flag;
  uint32_t m_no;
  int32_t m_health = 1;
  //exp_resi = 1,
  //hit_mask = 0, --HitMask
  //colli_attr = 0, --hitŽž’Ê’m‚·‚éattribute
  sf::Sprite m_spr;
};