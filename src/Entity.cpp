#include "stdafx.h"

#include "Resource.h"
#include "ObjLst.h"
#include "Entity.h"

Entity::Entity(const EntityArgs& args)
  : m_type(args.m_type)
  , m_pos(args.m_pos)
  , m_old_pos(args.m_pos)
{
  m_spr.setTexture(Resource::inst().get_pix_tex());
  ObjLst::inst().request(this);
}

Entity::~Entity()
{
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
}

void Entity::attr_bullet()
{
  ObjLst::inst().m_bullets.push_back(this);
}

void Entity::attr_ene_bullet()
{
  ObjLst::inst().m_ene_bullets.push_back(this);
}

void Entity::attr_ene_dot()
{
  ObjLst::inst().m_ene_dot.push_back(this);
}

void Entity::update(float dt)
{
}

void Entity::draw(sf::RenderWindow& window)
{
  m_spr.setOrigin(0.5f, 0.5f);
  m_spr.setScale(8, 8);
  m_spr.setPosition(m_pos);

  window.draw(m_spr);
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
  //if self.sha then
  //  self.sha:update(self, self.aabb0.x, self.aabb0.y, self.aabb_size.x, self.aabb_size.y)
  //end
}

void Entity::updateEstimateAABB() {
  //mov‚àŠÜ‚ß‚ÄXV
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
  //local bounce = 1.0 + BOUNCE
  //self.mov = self.mov + (inml * (-dist) * bounce)-- * (1.0 + bounce[i])); --bounce:1
  //local dot_vel_n = Vec2.Dot(self.vel, inml)
  //self.mov_old = self.mov_old + inml * dot_vel_n * bounce-- + dot_vel_n * inml-- * bounce
  //end
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

//end//Entity.sub_health_dmg = function(self, dmg)
//if self:check_flag(Flag_del) then return end
//self.health = self.health - dmg
//if self.health <= 0 then
//self : del()
//if self.dead then self : dead() end
//else
//self : set_blink()
//end
//end
//Entity.sub_health = function(self, t)
//self : sub_health_dmg(t.health)
//end
