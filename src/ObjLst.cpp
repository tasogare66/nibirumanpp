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
}

void ObjLst::draw(sf::RenderWindow& window)
{
  for (auto o:m_objs) {
    o->draw(window);
  }
}

void ObjLst::upd_move(float dt)
{
  for (auto o:m_objs) {
    o->update(dt);
  }
}
