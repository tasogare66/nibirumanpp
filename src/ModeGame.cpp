#include "stdafx.h"
#include "Player.h"

#include "ModeGame.h"

ModeGame::ModeGame()
{
}

ModeGame::~ModeGame()
{
}

void ModeGame::init()
{
  new Player();
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
