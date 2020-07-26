#include "stdafx.h"

#include "Entity.h"
#include "ObjLst.h"

void ObjLst::add(Entity* o)
{
  m_objs.push_back(o);
}

void ObjLst::update(float dt)
{
  this->upd_move(dt);
  this->upd_verlet(dt);
  m_prev_dt = dt;
}

void ObjLst::upd_verlet(float dt)
{
  auto prev_inv_dt = 1.0f / m_prev_dt;
  float damping = 0.4f; //‚È‚µ:1
  float decel = std::pow(std::abs(damping), dt);
  for (auto o: m_verlets) {
    o->do_verlet(dt, prev_inv_dt, decel);
  }
}

void ObjLst::upd_move(float dt)
{
  for (auto o : m_verlets) {
    o->pre_pro();
  }
  for (auto o:m_objs) {
    o->update(dt);
  }
}

void ObjLst::draw(sf::RenderWindow& window)
{
  for (auto o : m_objs) {
    o->draw(window);
  }
}
