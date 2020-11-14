#include "stdafx.h"

#include "Resource.h"
#include "ConstParam.h"
#include "ObjLst.h"
#include "Shash.h"
#include "Player.h"
#include "Random.h"
#include "GameSeq.h"

#include "Entity.h"

Entity::Entity(EntityType type, const EntityArgs& args)
  : m_type(type)
  , m_pos(args.m_pos)
  , m_old_pos(args.m_pos)
  , m_radius(args.m_radius)
  , m_mass(args.m_mass)
  , m_inv_mass(1.0f/args.m_mass)
  , m_aabb0(args.aabb0())
  , m_aabb_size(args.m_radius * 2.f, args.m_radius * 2.f)
  , m_half_extents(args.m_radius, args.m_radius)
{
  m_spr.setTexture(Resource::inst().get_spr_tex());
  this->spr8x8(m_dummy_spr_id);
  m_no = ObjLst::inst().request(this);
}

Entity::~Entity()
{
}

void Entity::set_sha(Shash* set_lst) {
  FW_ASSERT(!m_sha);
  m_sha = set_lst;
  m_sha->add(this, m_aabb0.x, m_aabb0.y, m_aabb_size.x, m_aabb_size.y);
}

void Entity::attr_verlet()
{
  FW_ASSERT(not m_flag.check(EntityFlag::AttrVerlet));
  ObjLst::inst().m_verlets.push_back(this);
  m_flag.on(EntityFlag::AttrVerlet);
}

void Entity::attr_px()
{
  this->attr_verlet();
  ObjLst::inst().m_pxs.push_back(this);
  this->set_sha(ObjLst::inst().m_px_sha.get());
}

void Entity::attr_bullet()
{
  this->attr_verlet();
  ObjLst::inst().m_bullets.push_back(this);
}

void Entity::attr_ene_bullet()
{
  this->attr_verlet();
  ObjLst::inst().m_ene_bullets.push_back(this);
  this->set_sha(ObjLst::inst().m_enblt_sha.get());
}

void Entity::attr_ene_dot()
{
  ObjLst::inst().m_ene_dot.push_back(this);
  this->set_sha(ObjLst::inst().m_endot_sha.get());
}

void Entity::attr_forces()
{
  ObjLst::inst().m_forces.push_back(this);
}

void Entity::attr_spawned()
{
  FW_ASSERT(not m_flag.check(EntityFlag::Spawned));
  m_flag.on(EntityFlag::Spawned);
  ++ObjLst::inst().m_spawn_ttl;
}

void Entity::update(float dt)
{
}

void Entity::draw(sf::RenderWindow& window)
{
  if (m_visible) {
    m_spr.setPosition(m_pos);

    FW_ASSERT(m_spr_id != m_dummy_spr_id);
    window.draw(m_spr);
  }
}

void Entity::set_radius(float ir)
{
  m_radius = ir;
  m_aabb_size.set(ir * 2.0f, ir * 2.0f);
  m_half_extents.set(ir, ir);
  m_aabb0 = m_pos - m_half_extents;
}

void Entity::set_mass(float imass)
{
  m_mass = imass;
  m_inv_mass = 1 / imass;
}

void Entity::set_position(const Vec2f& ipos) {
  m_pos = ipos;
  //updateAABB
  m_aabb0 = m_pos - m_half_extents;
  if (m_sha) {
    m_sha->update(this, m_aabb0.x, m_aabb0.y, m_aabb_size.x, m_aabb_size.y);
  }
}

void Entity::updateEstimateAABB() {
  //movも含めて更新
  m_aabb0 = m_pos + m_mov - m_half_extents;
}

void Entity::lim_vel_force(float l) {
  auto v = this->calc_velocity();
  auto len = v.magnitude();
  if (len > l) {
    m_old_pos = m_pos - v * l;
  }
}

void Entity::repulse(const Vec2f& inml, float dist)
{
  constexpr float bounce = 1.0f + const_param::BOUNCE;
  m_mov += (inml * (-dist) * bounce); // * (1.0 + bounce[i])); --bounce:1
  auto dot_vel_n = Vec2f::dot(m_vel, inml);
  m_mov_old += (inml * dot_vel_n * bounce); // + dot_vel_n * inml-- * bounce
}

void Entity::pre_pro()
{
  m_force.set(0.0f, 0.0f); //9 * self.mass
  m_mov.set(0.0f, 0.0f);
  m_mov_old.set(0.0f, 0.0f);
  this->set_velocity(this->calc_velocity());
}

void Entity::do_verlet(float dt, float inv_prev_dt, float decel)
{
  auto pos = m_pos;
  auto tmp = m_old_pos;
  pos += m_mov;
  tmp += m_mov;
  tmp = tmp + m_mov_old; //change velocity
  // do verlet
  auto vel = (pos - tmp) * inv_prev_dt;
  auto inv_mass_dt = m_inv_mass * dt;
  vel = vel + m_force * inv_mass_dt;
  vel = vel * decel;
  
  tmp = pos + vel * dt;
  
  this->set_position(tmp);
  m_old_pos = pos;
  this->set_velocity(vel);
}

//function Entity : topleft(cam)
//local p = self.pos + cam
//p.x = p.x + 0.5 - 4
//p.y = p.y + 0.5 - 4
//return p
//end

//Entity.drw = function(self, cam)
//local c = self:topleft(cam)
//spr(self.spr, c.x, c.y, 0)
//--circ(p.x + 0.5, p.y + 0.5, self.radius, 6)
//--print(string.format("%5.2f,%5.2f", self.pos.x, self.pos.y), p.x, p.y)
//end
//function Entity : drw_blink(cam)
//local c = self : topleft(cam)
//local s = self : is_blink() and 267 or self.spr
//spr(s, c.x, c.y, 0)
//end
//Entity.check_flag = function(self, f)
//return (self.flag & f ~= 0)
//end
//Entity.on_flag = function(self, f)
//self.flag = self.flag | f
//end
//Entity.off_flag = function(self, f)
//self.flag = self.flag & (~f)
//end
//Entity.chk_hitmask = function(self, m)
//return (self.hit_mask & m ~= 0)
//end
//Entity.off_hitmask = function(self, m)
//self.hit_mask = self.hit_mask & (~m)

void Entity::sub_health_dmg(int32_t dmg)
{
  if (m_flag.check(EntityFlag::Del) || dmg <= 0) return;
  m_health -= dmg;
  if (m_health <= 0) {
    this->del();
  } else {
    this->set_blink();
  }
}

void Entity::sub_health(const Entity* t) {
  this->sub_health_dmg(t->m_health);
}

void Entity::spr8x8(uint32_t id, uint16_t w, uint16_t h)
{
  auto rect = Resource::get_spr_rect(id);
  rect.width *= w;
  rect.height *= h;
  m_spr.setTextureRect(rect);
  m_spr_id = id;
  m_spr.setOrigin(static_cast<float>(rect.width)/2.0f, static_cast<float>(rect.height) / 2.0f);
}

bool Entity::check_kill_by_generated_player(std::function<void(int32_t i)> cb) const
{
  bool kill_by_player = false;
  for (int32_t i = 0; i < Entity_PLAYER_MAX; ++i) {
    if (m_hit_mask.check(Player::get_generated_player_hit_mask(i))) {
      kill_by_player = true;
      cb(i);
    }
  }
  return kill_by_player;
}

bool Entity::check_kill_by_player(std::function<void(int32_t i)> cb) const
{
  bool kill_by_player = false;
  for (int32_t i = 0; i < Entity_PLAYER_MAX; ++i) {
    if (m_hit_mask.check(Player::get_player_hit_mask(i))) {
      kill_by_player = true;
      cb(i);
    }
  }
  return kill_by_player;
}

Player* Entity::check_kill_by_player_random() const
{
  if (!m_hit_mask.check(HitMask::PlayerAll)) return nullptr;

  int32_t hit[Entity_PLAYER_MAX] = {};
  int32_t pt = 0;
  auto cbfunc = [&pt, &hit](int32_t idx) {
    hit[pt++] = idx;
  };
  if (this->check_kill_by_player(cbfunc)) {
    if (pt > 0) {
      auto r = (pt==1)?0 : rng::rand_int(pt - 1); //1体だとrandomしない
      return GameSeq::inst().get_player_entity_w(hit[r]);
    } else {
      FW_ASSERT(0);
    }
  } else {
    FW_ASSERT(0);
  }

  return nullptr;
}

template<>
const Player* Entity::cast_to<Player>() const
{
  if (m_type == EntityType::Player) return static_cast<const Player*>(this);
  return nullptr;
}
