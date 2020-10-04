#include "stdafx.h"

#include "ObjLst.h"
#include "ModeMng.h"
#include "Input.h"
#include "Camera.h"
#include "Resource.h"
#include "PtclLst.h"

#include "App.h"

App::App()
{
  Resource::inst().setup_at_boot();
  ModeMng::inst().request(ModeType::TITLE);
}

void App::update(float dt, sf::RenderWindow& window)
{
  // update input
  dt = Input::inst().update(dt, window);
  // update mode
  auto& modem = ModeMng::inst();
  if (modem.update(dt)) {
    ObjLst::inst().update(dt);
    PtclLst::inst().update(dt);
  }
  modem.update_post();
}

void App::draw(sf::RenderWindow& window)
{
  window.setView(Camera::inst().update_view());
  auto& modem = ModeMng::inst();
  modem.draw0(window);
  PtclLst::inst().draw(window);
  ObjLst::inst().draw(window);

  window.setView(Camera::inst().get_view_2d());
  modem.draw1(window);

  window.setView(window.getDefaultView());
  modem.draw2(window);
}
