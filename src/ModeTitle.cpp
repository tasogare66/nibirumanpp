#include "stdafx.h"

#include "Input.h"
#include "ModeMng.h"
#include "Random.h"
#include "GameUtil.h"
#include "ConstParam.h"
#include "Resource.h"
#include "GameSeq.h"

#include "ModeTitle.h"

int32_t ModeTitle::sCURSOR = 0;

ModeTitle::ModeTitle(ModeType in_mode_type)
  : Mode(in_mode_type)
{
}

ModeTitle::~ModeTitle()
{
}

void ModeTitle::init()
{
  //text
  m_text.setFont(Resource::inst().get_base_font());
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

  const auto& input = Input::inst();
  if (m_decided) {
    if (m_decided_time < 0) {
      // GAME = mode_game.new(self.decide_type)
      GameSeq::inst().set_entry_num(sCURSOR == 1 ? 2 : 1);
      ModeMng::inst().request(ModeType::GAME);
    }
    m_decided_time -= dt;
  }
  else if (input.decided()) {
    //self.decide_type = CURSOR == 0 and Input.StateLog or Input.StateTrace
    setdec();
  }
 // else if (btnp(7)) { //replay
    //self.decide_type = Input.StateTrace
    //  self : setdec()
 // }
  else if (input.trig(InputButton_Up)) {
    sCURSOR = std::max(sCURSOR-1,0);;
  }
  else if (input.trig(InputButton_Down) /*&& Input:exists_log()*/) {
    sCURSOR = std::min(sCURSOR+1,1);
  }
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
  //local t = "Nibiruman:2080"
  //  local y = 36
  //  local x = print_hcenter(t, y, 12, false, 2)
  //  print(t, x - 1, y, 12, false, 2)
  //  print(t, x, y - 1, 12, false, 2)
  //  local dur = self.decide and 6 or 26
  //  local r = self.elp // (FRAME2SEC*dur)
  //  if r % 2 == 0 then
  //    print_hcenter("- Press Z to start -", 108, 15, false, 1)
  //    end
  int rc = rng::range_int(1, 15);
  m_text.setCharacterSize(const_param::TXT_CHR_SIZE);
  m_text.setString("1P GAME START");
  auto px = gmutil::draw_text_center(window, m_text, 68.f * 5.f, sCURSOR == 0 ? rc : 15);
  m_text.setString("2P GAME START");
  gmutil::draw_text(window, m_text, px, 80.f * 5.f, sCURSOR == 1 ? rc : 15);
  m_text.setString("REPLAY");
  gmutil::draw_text(window, m_text, px, 92.f*5.f, sCURSOR == 2 ? rc : 15);
  //    print("REPLAY", x, 80, sCURSOR == 1 and rc or (Input:exists_log() and 15 or 10))
  //print_hcenter("HIGH SCORE", 2, 6, false, 1)
  //    print_hcenter(string.format("%d", HISCORE), 10, 15, true, 1)
  //    print(GAME_VER, 4, SCR_HEIGHT - 16, 7, false)
  //    print("@tasogare66 2020", 4, SCR_HEIGHT - 8, 7, false)
}
