﻿#include "stdafx.h"

#include "ConstParam.h"
#include "Entity.h"
#include "Shash.h"
#include "ObjLst.h"

ObjLst::ObjLst()
{
  m_px_sha = std::make_unique<Shash>(10);
  m_enblt_sha = std::make_unique<Shash>(10);
  m_endot_sha = std::make_unique<Shash>(10);
}

ObjLst::~ObjLst()
{
  for (auto o : m_request) {
    delete o;
  }
  for (auto o : m_objs) {
    delete o;
  }
}

void ObjLst::request(Entity* o)
{
  m_request.push_back(o);
}

void ObjLst::update(float dt)
{
  this->upd_move(dt);
  this->upd_del();
  this->upd_reciprocal();
  this->upd_colliders(m_pxs);
  this->upd_colliders(m_bullets, [](Entity* o, const Vec2f& dir) { return o->hit_wall(dir); });
  this->upd_colliders(m_ene_bullets, [](Entity* o, const Vec2f& dir) { return o->hit_wall(dir); });
  this->upd_verlet(dt);
  this->upd_add();
  m_prev_dt = dt;
}

void ObjLst::upd_colliders(std::vector<Entity*>& lst, std::function<bool(Entity*,Vec2f)> func)
{
  //inner circel
  constexpr auto lvradius = const_param::LvRadius;
  constexpr auto he = const_param::LvRadDr2 - 8.f;
  for (auto& obj : lst) {
    auto diff = -obj->get_estimate_pos();
    if (std::abs(diff.x) >= he || std::abs(diff.y) >= he) {
      auto len = diff.magnitude();
      auto d = len - lvradius + obj->get_radius();
      if (d > const_param::EPSILON) {
        auto dir = diff / len;
        if (func && func(obj, dir)) {
        } else {
          obj->repulse(dir, -d);
        }
      }
    }
  }
}

void ObjLst::upd_verlet(float dt)
{
  auto prev_inv_dt = 1.0f / m_prev_dt;
  float damping = 0.4f; //なし:1
  float decel = std::pow(std::abs(damping), dt);
  for (auto o: m_verlets) {
    o->do_verlet(dt, prev_inv_dt, decel);
  }
}

void ObjLst::upd_add()
{
  for (auto o:m_request) {
    o->init();
  }
  std::copy(m_request.begin(), m_request.end(), std::back_inserter(m_objs));
  m_request.clear();
}

void ObjLst::upd_del()
{
  auto del_obj_from_list = [](std::vector<Entity*>& lst, std::function<void(Entity*)> func=nullptr) {
    auto first = lst.begin();
    auto last = lst.end();
    auto result = first;
    for (; first != last; ++first) {
      if (not (*first)->get_flag().check(EntityFlag::del)) {
        if (first == result)
          ++result;
        else
          *result++ = std::move(*first);
      }
      else {
        if (func) func(*first);
      }
    }
    lst.erase(result, lst.end());
  };
  del_obj_from_list(m_pxs, [this](Entity* o) { m_px_sha->remove(o); });
  del_obj_from_list(m_bullets);
  del_obj_from_list(m_ene_bullets, [this](Entity* o) { m_enblt_sha->remove(o); });
  del_obj_from_list(m_ene_dot, [this](Entity* o) { m_endot_sha->remove(o); });
  del_obj_from_list(m_verlets);
  del_obj_from_list(m_objs, [](Entity* o) { delete o; });
}

void ObjLst::upd_move(float dt)
{
  for (auto o : m_verlets) {
    o->pre_pro();
  }
  for (auto o : m_objs) {
    o->update(dt);
  }
}

bool ObjLst::intersect_circle_vs_circle(const Entity* p1, const Entity* p2)
{
  auto diff = p2->get_pos() - p1->get_pos();
  float d = diff.magnitude();
  float target = p2->get_radius() + p1->get_radius();
  return (d > 0.0 && d < target);
}

void ObjLst::blt_vs_ene(Entity* o, Entity* b)
{
  constexpr auto flg = fw::underlying_cast(EntityFlag::Ally) | fw::underlying_cast(EntityFlag::del);
  if (o->m_flag.check(static_cast<EntityFlag>(flg))) return; //player, delは除く
  if (intersect_circle_vs_circle(o, b)) {
    o->sub_health(b);
    b->del();
  }
}

void ObjLst::upd_reciprocal()
{
  // bullet vs enemy
  for (auto* b : m_bullets) {
    auto& aabb0 = b->get_aabb0();
    float r2 = b->get_radius() * 2.f;
    m_px_sha->each(aabb0.x, aabb0.y, r2, r2,
      [&b](Entity* o) { blt_vs_ene(o, b); }
    );
  }
  // bullet vs ene_bullet
  for (auto* b : m_bullets) {
    auto& aabb0 = b->get_aabb0();
    float r2 = b->get_radius() * 2.f;
    m_enblt_sha->each(aabb0.x, aabb0.y, r2, r2,
      [&b](Entity* o) { blt_vs_ene(o, b); }
    );
  }
}

void ObjLst::draw(sf::RenderWindow& window)
{
  for (auto o : m_objs) {
    o->draw(window);
  }
}
