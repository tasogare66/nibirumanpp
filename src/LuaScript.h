#pragma once
enum class EnemyType;
struct EntityArgs;

namespace scr
{
  class ILuaScript {
  public:
    ILuaScript() = default;
    virtual ~ILuaScript() = default;

    virtual bool reset_thread(const char* in_co_func = nullptr) = 0;
    virtual void exec(float) = 0;
  };

  void spawn_base(EnemyType type, const EntityArgs& entity_args);

  // 敵生成用
  ILuaScript* create_lua_enemy_spawner();
}