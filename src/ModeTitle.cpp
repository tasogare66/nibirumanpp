#include "stdafx.h"

#include "Input.h"
#include "ModeMng.h"

#include "ModeTitle.h"

ModeTitle::ModeTitle(ModeType in_mode_type)
  : Mode(in_mode_type)
{
}

ModeTitle::~ModeTitle()
{
}

void ModeTitle::init()
{
}

void ModeTitle::dest()
{
}

bool ModeTitle::ctrl(float dt)
{
  auto setdec = [this]() {
    m_decided = true;
    m_decided_time = 1.f;
    //psfx(7, 'D-4', 30, 0);
  };

  m_elp += dt;
  if (m_decided) {
    if (m_decided_time < 0) {
     // GAME = mode_game.new(self.decide_type)
      ModeMng::inst().request(ModeType::GAME);
    }
    m_decided_time -= dt;
  }
  else if (Input::inst().decided()) {
    //self.decide_type = CURSOR == 0 and Input.StateLog or Input.StateTrace
    setdec();
  }
 // else if (btnp(7)) { //replay
    //self.decide_type = Input.StateTrace
    //  self : setdec()
 // }
 // else if (btn(0)) {
    //CURSOR = 0;
 // }
 // else if (btn(1) && Input:exists_log()) {
    //CURSOR = 1;
 // }
  return true;
}

void ModeTitle::ctrl_post()
{
}

void ModeTitle::draw0(sf::RenderWindow& window)
{
}

void ModeTitle::draw1(sf::RenderWindow& window)
{
}

void ModeTitle::draw2(sf::RenderWindow& window)
{
}
