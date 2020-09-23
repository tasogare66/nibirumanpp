﻿#include "stdafx.h"

#include "ConstParam.h"
#include "Input.h"
#include "ModeMng.h"
#include "Player.h"
#include "Reticle.h"
#include "Camera.h"
#include "ObjLst.h"
#include "GameSeq.h"
#include "Resource.h"

#include "ModeGame.h"

ModeGame::ModeGame(ModeType in_mode_type)
  : Mode(in_mode_type)
{
  constexpr auto lvrad = const_param::LvRadius;
  m_stg_circle.setRadius(lvrad);
  m_stg_circle.setOrigin(lvrad, lvrad);
  m_stg_circle.setFillColor(sf::Color(0));
  m_stg_circle.setOutlineThickness(1.f);
}

ModeGame::~ModeGame()
{
}

void ModeGame::base_clr()
{
  ObjLst::reset();
}

void ModeGame::upd_info(float dt)
{
}

void ModeGame::init()
{
  this->base_clr();
  GameSeq::inst().reset();
  m_spawner.init();
  //add player
  auto* reticle = new Reticle(EntityArgs{ Vec2f() });
  int32_t player_index = 0;
  auto p = new Player({ Vec2f() }, reticle, player_index);
  GameSeq::inst().add_player(p);
  //text
  m_text.setFont(Resource::inst().get_base_font());
  //sprite
  m_spr.setTexture(Resource::inst().get_spr_tex());
}

void ModeGame::dest()
{
  this->base_clr();
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
        //HISCORE = max(self.score, HISCORE)
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
  return ret;
}

void ModeGame::ctrl_post()
{
  Camera::inst().upd(GameSeq::inst().get_player_entities());
}

void ModeGame::draw0(sf::RenderWindow& window)
{
  window.draw(m_stg_circle);
}

void ModeGame::draw1(sf::RenderWindow& window)
{
  if (const auto* sp = GameSeq::inst().get_seq_player(0)) {
    const int32_t dsp_life_num = std::min(sp->get_life(), 10);
    m_spr.setTextureRect(Resource::get_spr_rect(481));
    for (int32_t i = 1; i < dsp_life_num; ++i) {
      m_spr.setPosition(i*8.f,0.f);
      window.draw(m_spr);
    }
  }
}

void ModeGame::draw2(sf::RenderWindow& window)
{
  auto lambda_draw_text = [this,&window](float ix, float iy, int32_t icol) {
    m_text.setFillColor(const_param::ticcol(icol));
    m_text.setPosition(ix, iy);
    window.draw(m_text);
  };

  char buf[256]={};
  constexpr auto txt_chr_size = 32 + 24;
  m_text.setCharacterSize(txt_chr_size);
  if (const auto* sp = GameSeq::inst().get_seq_player(0)) {
    snprintf(buf, fw::array_size(buf), "% 10lld", sp->get_score());
    m_text.setString(buf);
    m_text.setPosition(172.f/4.f, 32.f);
    window.draw(m_text);
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
      std::string_view gov = "GAME OVER";
      m_text.setString(gov.data());
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