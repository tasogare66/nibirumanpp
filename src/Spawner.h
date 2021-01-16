#pragma once
#include<memory>
struct lua_State;
namespace scr {
  class ILuaScript;
}
class Spawner {
public:
  Spawner();
  ~Spawner();
  void init();
  void reinit();
  void exec(float dt);
  lua_State* get_script_state() const;
private:
  std::unique_ptr<scr::ILuaScript> m_script;
};