#pragma once
enum class EnemyType;
struct EntityArgs;
class Boss;

namespace scr
{
  class ILuaScript {
  public:
    ILuaScript() = default;
    virtual ~ILuaScript() = default;

    virtual bool reset_thread(const char* in_co_func = nullptr) = 0;
    virtual void exec(float) = 0;
  };

  //一部entity_args更新する
  void spawn_base(EnemyType type, EntityArgs& entity_args);

  // 敵生成用
  ILuaScript* create_lua_enemy_spawner();
  // boss用
  ILuaScript* create_lua_boss_sequence(std::string_view in_co_str, Boss* boss);
}