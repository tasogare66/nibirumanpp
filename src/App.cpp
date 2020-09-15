#include "stdafx.h"

#include "ObjLst.h"
#include "ModeMng.h"
#include "Input.h"
#include "Camera.h"
#include "Resource.h"
#include "App.h"

App::App()
{
  Resource::inst().setup_at_boot();
  ModeMng::inst().request(ModeType::GAME);
}

void App::update(float dt, sf::RenderWindow& window)
{
  // update input
  dt = Input::inst().update(dt, window);
  // update mode
  auto& modem = ModeMng::inst();
  if (modem.update(dt)) {
    ObjLst::inst().update(dt);
  }
  modem.update_post();
}

void App::draw(sf::RenderWindow& window)
{
  window.setView(Camera::inst().update_view());
  auto& modem = ModeMng::inst();
  {
    modem.draw0(window);
    ObjLst::inst().draw(window);
    modem.draw1(window);
  }
  window.setView(window.getDefaultView());

  modem.draw2(window);
}
