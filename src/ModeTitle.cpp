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

  int w = static_cast<int32_t>(const_param::SCR_WIDTH);
  int h = static_cast<int32_t>(const_param::SCR_HEIGHT);
  constexpr int32_t num = 2000;
  m_p.resize(num);
  for (auto& p:m_p) {
    p = { rng::rand_int(w) - h, rng::rand_int(w) - h, rng::rand_int(w) };
  }
  //pix tex
  m_spr.setTexture(Resource::inst().get_pix_tex());
  m_spr.setOrigin(0.5f, 0.5f);
  m_spr.setColor(const_param::ticcol(10));
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
  float w = const_param::SCR_WIDTH;
  float h = const_param::SCR_HEIGHT;
  for (auto& p :m_p) {
    if (p.z > 0) {
      float x = static_cast<float>(p.x) / p.z * w;
      float y = static_cast<float>(p.y) / p.z * w;
      //pix(x, y, 10)
      m_spr.setPosition(x, y);
      window.draw(m_spr);
    }
    auto mod = [](int x, int divisor)->int {
      int m = x % divisor;
      return m + (m < 0 ? divisor : 0);
    };
    p.z = mod(p.z, static_cast<int>(w)) - 1;
  }
}

void ModeTitle::draw1(sf::RenderWindow& window)
{
}

void ModeTitle::draw2(sf::RenderWindow& window)
{
  m_text.setCharacterSize(62*2);
  m_text.setString("Nibiruman:2080");
  gmutil::draw_text_center(window, m_text, 16.f*8.f, 12);
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
  auto print_hcenter = [this,&window](const char* str,float y, int32_t col) {
    m_text.setString(str);
    gmutil::draw_text_center(window, m_text, y, col);
  };
  print_hcenter("HIGH SCORE", -6.f, 6);
  char buf[128];
  sprintf_s(buf, fw::array_size(buf), "%lld", GameSeq::inst().get_hiscore());
  print_hcenter(buf, -6.f+42.f, 15); //string.format("%d", HISCORE), 10, 15, true, 1)
  //    print(GAME_VER, 4, SCR_HEIGHT - 16, 7, false)
  //    print("@tasogare66 2020", 4, SCR_HEIGHT - 8, 7, false)
}
