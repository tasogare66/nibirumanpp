#include <SFML\Graphics.hpp>
#include "ObjLst.h"
#include "Entity.h"
#include "App.h"

App::App()
{
  new Entity();
}

void App::update()
{
  ObjLst::inst().update();
}

void App::draw(sf::RenderWindow& window)
{
  ObjLst::inst().draw(window);
}
