#include "stdafx.h"

#include "ConstParam.h"
#include "Player.h"
#include "Reticle.h"
#include "Camera.h"
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

void ModeGame::init()
{
  GameSeq::inst().reset();
  //add player
  auto* reticle = new Reticle({ EntityType::None,Vec2f() });
  auto p = new Player({ EntityType::Player,Vec2f() }, reticle);
  GameSeq::inst().add_player(p);
}

void ModeGame::dest()
{
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
