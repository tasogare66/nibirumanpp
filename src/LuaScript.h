#pragma once
enum class EnemyType;
struct EntityArgs;
class Enemy;
class Boss;
struct lua_State;

namespace scr
{
  class ILuaScript {
  public:
    ILuaScript() = default;
    virtual ~ILuaScript() = default;

    virtual lua_State* state() const = 0;
    virtual bool reset_thread(const char* in_co_func = nullptr) = 0;
    virtual void exec(float) = 0;
  };

  //一部entity_args更新する
  //scriptからの敵生成は spawn_via_scr
  Enemy* spawn(EnemyType type, EntityArgs& entity_args);

  // 敵生成用
  ILuaScript* create_lua_enemy_spawner();
  // boss用
  ILuaScript* create_lua_boss_sequence(std::string_view in_co_str, Boss* boss);
  void spawn_for_boss_urchin(int32_t type);
}