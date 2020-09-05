#pragma once
#include<memory>
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
private:
  std::unique_ptr<scr::ILuaScript> m_script;
};