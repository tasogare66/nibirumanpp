#include "stdafx.h"

#include <functional>

#include "Entity.h"
#include "ObjLst.h"

void ObjLst::request(Entity* o)
{
  m_request.push_back(o);
}

void ObjLst::update(float dt)
{
  this->upd_move(dt);
  this->upd_del();
  this->upd_verlet(dt);
  this->upd_add();
  m_prev_dt = dt;
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
  del_obj_from_list(m_pxs, nullptr);// , function(o) self.PX_SHA:remove(o) end)
  del_obj_from_list(m_bullets);
  del_obj_from_list(m_ene_bullets);// , function(o) self.ENBLT_SHA:remove(o) end)
  del_obj_from_list(m_ene_dot);// , function(o) self.ENDOT_SHA:remove(o) end)
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

void ObjLst::draw(sf::RenderWindow& window)
{
  for (auto o : m_objs) {
    o->draw(window);
  }
}
