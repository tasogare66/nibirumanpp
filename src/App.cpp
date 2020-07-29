#include "stdafx.h"

#include "ObjLst.h"
#include "ModeMng.h"
#include "Input.h"
#include "App.h"

App::App()
{
  ModeMng::inst().request(ModeType::GAME);
}

void App::update(float dt)
{
  // update input
  dt = Input::inst().update(dt);
  // update mode
  auto& modem = ModeMng::inst();
  if (modem.update(dt)) {
    ObjLst::inst().update(dt);
  }
  modem.update_post();
}

void App::draw(sf::RenderWindow& window)
{
  auto& modem = ModeMng::inst();
  modem.draw0(window);
  ObjLst::inst().draw(window);
  modem.draw1(window);
}
