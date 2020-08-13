#include "stdafx.h"

#include "ConstParam.h"
#include "Player.h"
#include "Reticle.h"
#include "EneGrunt.h" //test  
#include "Camera.h"
#include "ObjLst.h"
#include "GameSeq.h"
#include "ModeGame.h"

ModeGame::ModeGame()
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
  //add player
  auto* reticle = new Reticle({ EntityType::None,Vec2f() });
  auto p = new Player({ EntityType::Player,Vec2f() }, reticle);
  GameSeq::inst().add_player(p);

  new EneGrunt({EntityType::Enemy, Vec2f(50.f,50.f) });
}

void ModeGame::dest()
{
  this->base_clr();
}

bool ModeGame::ctrl(float dt)
{
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
