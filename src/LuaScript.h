#pragma once

namespace scr
{
  class ILuaScript {
  public:
    ILuaScript() = default;
    virtual ~ILuaScript() = default;

    virtual bool reset_thread(const char* in_co_func = nullptr) = 0;
    virtual void exec(float) = 0;
  };

  // ìGê∂ê¨óp
  ILuaScript* create_lua_spawner();
}