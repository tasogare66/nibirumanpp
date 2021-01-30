#include "stdafx.h"

#include "ObjLst.h"
#include "ModeMng.h"
#include "Input.h"
#include "Camera.h"
#include "Resource.h"
#include "PtclLst.h"
#include "Sound.h"
#include "EntityData.h"
#include "SaveData.h"

#include "App.h"

App::App()
{
  Resource::inst().setup_at_boot();
  EntityDataMng::inst().setup_at_boot();
  Sound::inst().setup_at_boot();
  SaveDataMng::inst().setup_at_boot(); //最後に
  ModeMng::inst().request(ModeType::TITLE);
}

void App::update_app(float dt, sf::RenderWindow& window)
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

bool App::update(float dt, sf::RenderWindow& window)
{
#if DEBUG
  if (SaveDataMng::deb_conf().m_fix_frame_time) {
    dt = const_param::FRAME2SEC; //固定frame
  }
#endif
  dt = std::min(dt, const_param::FRAME2SEC*10.f); //10frame以上のskipはいれない

  auto& input{ Input::inst() };
  input.update_system();
  bool dbg_step = false;
#if DEBUG //debug pause
  if (m_dbg_pause) {
    if (input.dbg_pause_cancel()) {
      m_dbg_pause = false;
    } else {
      dbg_step = input.dbg_pause();
    }
  } else if (input.dbg_pause()) {
    m_dbg_pause = true;
  }
#endif
  if (!m_dbg_pause || dbg_step) {
    this->update_app(dt, window);
  }

#if DEBUG
  return !input.dbg_escape(); //ESCで終了
#else
  return true;
#endif
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
