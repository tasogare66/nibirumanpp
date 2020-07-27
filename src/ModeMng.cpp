#include "stdafx.h"

#include "Mode.h"
#include "ModeGame.h"
#include "ModeMng.h"

ModeMng::ModeMng()
{
}

ModeMng::~ModeMng()
{
}

void ModeMng::request(ModeType req)
{
  switch (req) {
  case ModeType::GAME:
    m_req_work = std::make_unique<ModeGame>();
    break;
  case ModeType::TITLE:
  default:
    FW_ASSERT(0);
    break;
  }
}

bool ModeMng::update(float dt)
{
  if (m_req_work) {
    this->dest();
    m_cur_work = std::move(m_req_work);
    m_req_work.reset();
    this->init();
  }
  return this->ctrl(dt);
}

void ModeMng::update_post()
{
  if (m_cur_work) {
    m_cur_work->ctrl_post();
  }
}

void ModeMng::draw0(sf::RenderWindow& window)
{
  if (m_cur_work) {
    m_cur_work->draw0(window);
  }
}

void ModeMng::draw1(sf::RenderWindow& window)
{
  if (m_cur_work) {
    m_cur_work->draw1(window);
  }
}

void ModeMng::init()
{
  if (m_cur_work) {
    m_cur_work->init();
  }
}

void ModeMng::dest()
{
  if (m_cur_work) {
    m_cur_work->dest();
  }
}

bool ModeMng::ctrl(float dt)
{
  if (m_cur_work) {
    return m_cur_work->ctrl(dt);
  }
  return false;
}
