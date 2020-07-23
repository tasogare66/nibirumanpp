#include <SFML\Graphics.hpp>
#include "Entity.h"
#include "ObjLst.h"

void ObjLst::add(Entity* o)
{
  m_objs.push_back(o);
}

void ObjLst::update()
{
  this->upd_move();
}

void ObjLst::draw(sf::RenderWindow& window)
{
  for (auto o:m_objs) {
    o->draw(window);
  }
}

void ObjLst::upd_move()
{
  for (auto o:m_objs) {
    o->update();
  }
}
