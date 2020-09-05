#include "stdafx.h"

#include "ConstParam.h"
#include "Player.h"
#include "Reticle.h"
#include "Camera.h"
#include "ObjLst.h"
#include "GameSeq.h"
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
  auto p = new Player({ Vec2f() }, reticle);
  GameSeq::inst().add_player(p);
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
