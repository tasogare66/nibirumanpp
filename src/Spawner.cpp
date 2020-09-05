#include "stdafx.h"

#include "LuaScript.h"

#include "Spawner.h"

Spawner::Spawner()
{
}

Spawner::~Spawner() = default;

void Spawner::init()
{
  m_script.reset(scr::create_lua_enemy_spawner());
  m_script->reset_thread();
}

void Spawner::reinit()
{
  this->init();
}

void Spawner::exec(float dt)
{
  if (m_script) {
    m_script->exec(dt);
  }
}
