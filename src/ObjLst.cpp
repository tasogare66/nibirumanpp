#include "stdafx.h"

#include "ConstParam.h"
#include "Entity.h"
#include "Shash.h"
#include "GameSeq.h"
#include "Player.h"
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

uint32_t ObjLst::request(Entity* o)
{
  m_request.push_back(o);
  return ++m_cumulative_no;
}

void ObjLst::update(float dt)
{
  this->upd_move(dt);
  this->upd_del(); //1st
  this->upd_prepare();
  this->upd_reciprocal();
  this->upd_colliders(m_pxs);
  this->upd_colliders(m_bullets, [](Entity* o, const Vec2f& dir) { return o->hit_wall(dir); });
  this->upd_colliders(m_ene_bullets, [](Entity* o, const Vec2f& dir) { return o->hit_wall(dir); });
  this->upd_verlet(dt);
  this->upd_del(); //2nd
  this->upd_add();
  m_prev_dt = dt;
}

template<typename T>
void ObjLst::upd_colliders(std::vector<T*>& lst, std::function<bool(Entity*,Vec2f)> func)
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

template<typename T>
void del_obj_from_list(std::vector<T*>& lst, std::function<void(Entity*)> func=nullptr) {
  auto first = lst.begin();
  auto last = lst.end();
  auto result = first;
  for (; first != last; ++first) {
    if (not (*first)->get_flag().test(EntityFlag::Del)) {
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
}

void ObjLst::upd_del()
{
  del_obj_from_list(m_pxs, [this](Entity* o) { m_px_sha->remove(o); });
  del_obj_from_list(m_bullets);
  del_obj_from_list(m_ene_bullets, [this](Entity* o) { m_enblt_sha->remove(o); });
  del_obj_from_list(m_ene_dot, [this](Entity* o) { m_endot_sha->remove(o); });
  del_obj_from_list(m_verlets);
  del_obj_from_list(m_objs, [](Entity* o) { o->dead(); delete o; });
}

void ObjLst::upd_prepare()
{
  //clear hit_mask
  for (auto o : m_objs) {
    o->m_hit_mask.reset();
  }
}

void ObjLst::upd_move(float dt)
{
  for (auto o : m_verlets) {
    o->pre_pro();
  }
  m_spawn_num = 0;
  for (auto o : m_objs) {
    if (o->m_flag.test(EntityFlag::UpdateEnabled)) {
      o->update(dt);
    }
    if (o->m_flag.test(EntityFlag::Spawned)) ++m_spawn_num;
  }
}

bool ObjLst::intersect_circle_vs_circle(const Entity* p1, const Entity* p2)
{
  auto diff = p2->get_pos() - p1->get_pos();
  float d = diff.magnitude();
  float target = p2->get_radius() + p1->get_radius();
  return (d < target);
}

void ObjLst::reciprocal_each(Entity* p1, Entity* p2)
{
  auto diff = p2->m_pos - p1->m_pos;
  float sqr_d = diff.sqr_magnitude();
  float d = std::sqrt(sqr_d);
  float target = p2->m_radius + p1->m_radius;
  if (d > 0.0f && d < target) { // d==0: same particle
    auto factor = (d - target) / d * 0.5f;
    constexpr float ebounce = const_param::BOUNCE;
    p1->m_mov += diff * factor;
    p2->m_mov -= diff * factor;
    // preserve impulse
    float inv_sqr_d = 1.0f / sqr_d;
    auto f1 = ebounce * Vec2f::dot(p1->m_vel, diff) * inv_sqr_d; //mass:1
    auto f2 = ebounce * Vec2f::dot(p2->m_vel, diff) * inv_sqr_d;
    auto f1f2 = f1 - f2;
    // p1
    p1->m_mov_old += diff * f1f2 * p1->m_inv_mass;
    p1->m_hit_mask.set(p2->m_colli_attr);
    // p2
    p2->m_mov_old -= diff * f1f2 * p2->m_inv_mass;
    p2->m_hit_mask.set(p1->m_colli_attr);
  }
}

void ObjLst::blt_vs_ene(Entity* o, Entity* b)
{
  //constexpr auto flg = fw::underlying_cast(EntityFlag::Ally) | fw::underlying_cast(EntityFlag::del);
  constexpr auto flg = fw::underlying_cast(EntityFlag::Ally);
  if (o->m_flag.test(static_cast<EntityFlag>(flg))) return; //playerは除く
  if (intersect_circle_vs_circle(o, b)) {
    o->m_hit_mask.set(b->m_colli_attr);
    if (o->sub_health(b)) {
      o->play_blink_sfx();
    }
    b->del();
  }
}

void ObjLst::force_vs_ene(Entity* o, const Entity* f)
{
  constexpr auto flg = fw::underlying_cast(EntityFlag::Ally);
  if (o->m_flag.test(static_cast<EntityFlag>(flg))) return; //playerは除く
  // intersect_circle_vs_circle
  auto diff = o->get_pos() - f->get_pos();
  float d = diff.magnitude();
  float target = f->get_radius() + o->get_radius();
  if (d < target) {
    f->hitcb_w(o, diff, d); //oに対して設定
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
    const auto& aabb0 = b->get_aabb0();
    float r2 = b->get_radius() * 2.f;
    m_enblt_sha->each(aabb0.x, aabb0.y, r2, r2,
      [&b](Entity* o) { blt_vs_ene(o, b); }
    );
  }

  // force vs enemy
  for (const auto *f : m_forces) {
    const auto& aabb0 = f->get_aabb0();
    float r2 = f->get_radius() * 2.f;
    // vs enemy
    m_px_sha->each(aabb0.x, aabb0.y, r2, r2,
      [&f](Entity* o) { force_vs_ene(o, f); }
    );
    // vs ene_bullet
    m_enblt_sha->each(aabb0.x, aabb0.y, r2, r2,
      [&f](Entity* o) { force_vs_ene(o, f); }
    );
  }
  m_forces.clear();

  auto& pls = GameSeq::inst().get_player_entities();
  // player vs ene_bullet
  for (const auto& pl : pls) {
    if (pl->m_flag.test(EntityFlag::CollisionEnabled)) {
      const auto& pl_aabb0 = pl->get_aabb0();
      const auto pl_radius2 = pl->get_radius() * 2.0f;
      m_enblt_sha->each(pl_aabb0.x, pl_aabb0.y, pl_radius2, pl_radius2,
        [&pl](Entity* o) {
        if (o->m_flag.test(EntityFlag::Del)) return; //delは除く
        if (intersect_circle_vs_circle(o, pl)) {
          pl->m_hit_mask.set(o->m_colli_attr);
          o->m_hit_mask.set(pl->m_colli_attr);
        }
      });
    }
  }
  // player vs enedot
  for (const auto& pl : pls) {
    auto capr = pl->get_capradius();
    auto pl_pos = pl->get_pos();
    m_endot_sha->each(pl_pos.x - capr, pl_pos.y - capr, capr * 2.0f, capr * 2.0f,
      [&](Entity* o) {
      //if (o->m_flag.check(EntityFlag::Del)) return; //delは除く
      auto diff = o->m_pos - pl->m_pos;
      auto d = diff.magnitude();
      auto target = capr + o->m_radius;
      if (d < target) {
        o->hitcb(pl, Vec2f(), d - pl->m_radius);
      }
    });
  }
  // obj同士
  for (auto* obj : m_pxs) {
    if (obj->m_flag.test(EntityFlag::CollisionEnabled)) {
      m_px_sha->each(obj, [&](Entity* o) {
        if (obj->m_no > o->m_no && (o->m_flag.test(EntityFlag::CollisionEnabled))) {
          //if (o->m_flag.test(EntityFlag::IgnoreCollisionSameRoot) && o->m_root == obj->m_root) return;
          reciprocal_each(obj, o);
        }
      });
    }
  }
}

void ObjLst::draw(sf::RenderWindow& window)
{
  //reverse order
  if (m_objs.empty()) return;
  const auto num = m_objs.size();
  auto o = &m_objs[num - 1];
  for (auto i = 0; i < num; ++i,--o) {
    (*o)->draw(window);
  }
}

#if DEBUG
void ObjLst::del_all_enemies()
{
  for (auto o : m_objs) {
    if (o->m_type == EntityType::Enemy) {
      o->suiside(); //delマーク付ける
    }
  }
}
#endif