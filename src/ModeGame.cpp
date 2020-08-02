#include "stdafx.h"

#include "Player.h"
#include "Reticle.h"
#include "ModeGame.h"

ModeGame::ModeGame()
{
}

ModeGame::~ModeGame()
{
}

void ModeGame::init()
{
  auto* recicle = new Reticle({ EntityType::None,Vec2f() });
  new Player({ EntityType::Player,Vec2f() });
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
}

void ModeGame::draw0(sf::RenderWindow& window)
{
}

void ModeGame::draw1(sf::RenderWindow& window)
{
}
