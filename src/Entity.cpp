#include <SFML\Graphics.hpp>
#include "Resource.h"
#include "ObjLst.h"
#include "Entity.h"

Entity::Entity()
{
  m_spr.setTexture(Resource::inst().get_pix_tex());
  ObjLst::inst().add(this);
}

Entity::~Entity()
{
}

void Entity::update()
{
}

void Entity::draw(sf::RenderWindow& window)
{
  m_spr.setScale(20,20);
  m_spr.setPosition(150,100);
  window.draw(m_spr);
}
