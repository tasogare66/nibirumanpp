#include "stdafx.h"

#include "ObjLst.h"
#include "Entity.h"
#include "App.h"

App::App()
{
  new Entity();
}

void App::update(float dt)
{
  ObjLst::inst().update(dt);
}

void App::draw(sf::RenderWindow& window)
{
  ObjLst::inst().draw(window);
}
