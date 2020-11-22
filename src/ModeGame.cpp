#include "stdafx.h"

#include "ConstParam.h"
#include "Input.h"
#include "ModeMng.h"
#include "Player.h"
#include "Reticle.h"
#include "Camera.h"
#include "ObjLst.h"
#include "GameSeq.h"
#include "Resource.h"
#include "GameUtil.h"
#include "Boss.h"

#include "ModeGame.h"

ModeGame::ModeGame(ModeType in_mode_type)
  : Mode(in_mode_type)
{
}

ModeGame::~ModeGame()
{
}

void ModeGame::upd_info(float dt)
{
  GameSeq::inst().update_info(dt);
}

void ModeGame::init()
{
  GameSeq::inst().reset();
  m_spawner.init();
  const auto entry_num = GameSeq::inst().get_entry_num();
  Input::inst().update_assignment(entry_num, true);
  FW_ASSERT(entry_num<=2);
  const Vec2f entry_1[] = { Vec2f(0.f,0.f) };
  const Vec2f entry_2[] = { Vec2f(-15.f,0.f), Vec2f(15.0f,0.f) };
  auto* ptbl = (entry_num == 1) ? entry_1 : entry_2;
  for (uint32_t player_index = 0; player_index < entry_num; ++player_index) {
    //add player
    auto* reticle = (player_index == 0) ? new Reticle(EntityArgs{ Vec2f() }, player_index) : nullptr;
    auto p = new Player({ ptbl[player_index] }, reticle, player_index);
    GameSeq::inst().add_player(p);
  }
  GameSeq::inst().setup_game();
  //text
  m_text.setFont(Resource::inst().get_base_font());
  //sprite
  m_spr.setTexture(Resource::inst().get_spr_tex());
}

void ModeGame::dest()
{
}

bool ModeGame::ctrl(float dt)
{
  bool ret = true;
  if (m_state == State::Entry) {
    //GAME.pl:set_active(self.enttm < 0)
    if (m_enttm + m_entstrx < 0.f) {
      m_state = State::Play;
    }
    m_enttm -= (dt * 190.f);
  }
  if (m_state == State::Play) {
    auto func = [this,dt]() {
      if (GameSeq::inst().check_game_over()) {
        m_state = State::Over;
        //Input: term()
        GameSeq::inst().update_hiscore();
        return;
      }
      m_spawner.exec(dt);
      this->upd_info(dt);
    };
    func();
  }
  if (m_state == State::Over) {
    if (Input::inst().decided()) {
      ModeMng::inst().request(ModeType::TITLE);
    }
    m_ovelp += dt;
    ret = false;
  }
  if (ret) {
    //更新時のみentityの参照クリア
    this->clear_bossrf();
  }
  return ret;
}

void ModeGame::ctrl_post()
{
  Camera::inst().upd(GameSeq::inst().get_player_entities());
}

void ModeGame::draw0(sf::RenderWindow& window)
{
  m_bg.draw(window);
}

void ModeGame::draw1(sf::RenderWindow& window)
{
  if (const auto* sp = GameSeq::inst().get_seq_player(0)) {
    const int32_t dsp_life_num = std::min(sp->get_life(), 10);
    m_spr.setTextureRect(Resource::get_spr_rect(481));
    for (int32_t i = 0; i < dsp_life_num; ++i) {
      m_spr.setPosition(i*8.f, 1.f);
      window.draw(m_spr);
    }
  }
}

void ModeGame::draw2(sf::RenderWindow& window)
{
  if (m_boss) m_boss->draw_info(window);

  auto lambda_draw_text = [this,&window](float ix, float iy, int32_t icol) {
    gmutil::draw_text(window, m_text, ix, iy, icol);
  };

  char buf[256]={};
  constexpr auto txt_chr_size = const_param::TXT_CHR_SIZE;
  m_text.setCharacterSize(txt_chr_size);
  if (const auto* sp = GameSeq::inst().get_seq_player(0)) {
    const auto disp_score = sp->get_score();
    int digit_num = disp_score<=0 ? 1 : static_cast<int>(std::log10(disp_score)+1); //桁数
    for (int i = 0; i < digit_num;++i) {
      buf[i] = '0';
    }
    buf[digit_num] = 0; //終端
    m_text.setString(buf);
    auto bound = m_text.getLocalBounds(); //0埋めてサイズ出す

    snprintf(buf, fw::array_size(buf), "%lld", sp->get_score());
    m_text.setString(buf);
    lambda_draw_text(const_param::WND_WIDTH - 32.f - bound.width, 0.f, 15);

    int col[] = { 6,15,11 };
    uint8_t c = 1;
    if (sp->get_multiplier() > 1.0f) {
      c = std::min(static_cast<int>(std::floor(sp->get_multime() / sp->m_multimeLimit * 3.f)),2);
    }
    FW_ASSERT(c<fw::array_size(col));
    snprintf(buf, fw::array_size(buf), "%5.2fx", sp->get_multiplier());
    m_text.setString(buf);
    bound = m_text.getLocalBounds();
    lambda_draw_text(const_param::WND_WIDTH - 64.f - bound.width, 44.f, col[c]);
  }

  if (m_state == State::Entry) {
    //local y, merg = 48, 2
    //  rect(0, y - merg, SCR_WIDTH, 5 + merg * 2, 2)
    //  local str = "SAVE THE LAST HUMANS"
    //  local x = self.enttm
    //  self.entstrx = print(str, x, y, 11)
  }
  else if (m_state == State::Over) {
    auto r = static_cast<int32_t>(m_ovelp / (const_param::FRAME2SEC * 26.f));
    if (r % 2 == 0) {
      m_text.setString("GAME OVER");
      auto bound = m_text.getLocalBounds();
      float x = (const_param::WND_WIDTH - bound.width) / 2.0f;
      float y = ((128 - 8) / 2)*6.f;
      constexpr auto ofs = 3.f;
      lambda_draw_text(x - ofs, y - ofs, 3);
      lambda_draw_text(x + ofs, y + ofs, 3);
      lambda_draw_text(x,y,15);
    }
  }
}