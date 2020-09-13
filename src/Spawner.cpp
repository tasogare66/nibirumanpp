#include "stdafx.h"

#include "LuaScript.h"
#include "DwGui.h"
#include "Vec2.h"
#include "Entity.h"

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
#if DEBUG
  auto enemy_type = DwGui::inst().check_spawn_req();
  if (enemy_type >= 0) {
    EntityArgs args;
    args.m_pos.set(60.f,-30.f);
    scr::spawn_base(static_cast<EnemyType>(enemy_type), args);
  }
  if (DwGui::inst().get_disable_spawn_scritp()) return;
#endif
  if (m_script) {
    m_script->exec(dt);
  }
}
