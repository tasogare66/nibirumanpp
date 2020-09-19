#include "stdafx.h"

#include "ConstParam.h"
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
  m_score_text.setFont(Resource::inst().get_base_font());
}

void ModeGame::dest()
{
  this->base_clr();
}

bool ModeGame::ctrl(float dt)
{
  m_spawner.exec(dt);
  return true;
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
}

void ModeGame::draw2(sf::RenderWindow& window)
{
  char buf[256]={};
  if (const auto* sp = GameSeq::inst().get_seq_player(0)) {
    snprintf(buf, fw::array_size(buf), "% 10lld", sp->get_score());
    m_score_text.setCharacterSize(32);
    m_score_text.setString(buf);
    m_score_text.setPosition(172.f/4.f, 0.f);
    window.draw(m_score_text);
  }
}