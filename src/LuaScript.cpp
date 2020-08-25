#include "stdafx.h"

// warning対処
#pragma warning( push )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4189 )

#include <lua.hpp>
#include <LuaIntf/LuaIntf.h>

#pragma warning( pop )

#include "LuaScript.h"

namespace LuaIntf
{
  // Enable support for shared_ptr, vector and map.
  // This will allow us to use shared_ptr almost interchangeably with raw pointers,
  // as well as use Lua tables to handle C++ vectors and maps
  LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
  LUA_USING_LIST_TYPE(std::vector)
  LUA_USING_MAP_TYPE(std::map)
}

namespace scr
{
  using namespace LuaIntf;

  class LuaScriptBase : public ILuaScript {
  public:
    explicit LuaScriptBase(const char* in_co_str)
      : ILuaScript()
      , m_co_str(in_co_str)
    {}
    virtual ~LuaScriptBase() = default;

    // lua読み込んでセットアップ
    void load_and_setup(const std::string_view& in_scr_name)
    {
      std::string fname(m_script_dir);
      fname += in_scr_name;
      //luaL_openlibs(m_ctx.state());
      m_ctx.importLibs();
      try {
        this->bind_base(); // 全scriptに提供するもの
        this->bind(); // 各scriptで個別にbind
        m_ctx.doFile(fname.c_str());
      }
      catch (const LuaIntf::LuaException& e) {
        this->set_error(e);
      }
    }

    virtual bool reset_thread(const char* in_co_func) override
    {
      const char* co_func = in_co_func;
      if (co_func == nullptr) {
        if (m_co_str) {
          co_func = m_co_str; // restart
        }
        else {
          FW_ASSERT(0);
          return false;
        }
      }
      LuaIntf::LuaState state(m_ctx.state());
      m_thread = state.newThread();
      m_thread.getGlobal(co_func);
      m_co_str = co_func;
      m_end_flag = false;
      return true;
    }

    virtual void exec(float dt) override
    {
      if (m_end_flag == false && m_error_flag == false)
      {
        // 個別の更新処理
        this->update(dt);

        try {
          int ret = m_thread.resume(0);
          switch (ret) {
          case LUA_OK:
          case LUA_YIELD:
            break;
          default:
            throw LuaIntf::LuaException(m_thread);
            break;
          }
        }
        catch (const LuaIntf::LuaException& e) {
          this->set_error(e);
        }

        if (auto ret = m_thread.toInteger(-1)) {
          // 終了
          m_end_flag = true;
        }
      }
    }

  protected:
    virtual void bind() = 0;
    virtual void update(float dt) = 0;

  private:
    void bind_base()
    {
      LuaBinding(m_ctx.state()).beginModule("proj")
        .addFunction("error", [this](std::string err) {
        this->set_error(err.c_str());
      }).addFunction("log", [this](std::string log) {
        this->set_log(log.c_str());
      }).endModule();

      //LuaBinding(m_ctx.state()).beginModule("game")
      //  .addFunction("random_range", [this](float min, float max) {
      //  return fw::random::random_range(min, max, m_random_type);
      //}).addFunction("random_int", [this](int min, int max) {
      //  return fw::random::random_int(min, max, m_random_type);
      //}).endModule();
    }

    void set_log(const char* log)
    {
      //if (gm::PlayImpl::singleton().m_script_log) {
      //  gm::PlayImpl::singleton().m_script_log->AddLog("[LOG]:%s\n", log);
      //}
    }

    void set_error(const char* err)
    {
      //if (gm::PlayImpl::singleton().m_script_log && err) {
      //  gm::PlayImpl::singleton().m_script_log->AddLog("[ERR]:%s\n", err);
      //}
      m_error_flag = true;
      FW_ASSERT(m_assert_flag);
    }

    void set_error(const LuaIntf::LuaException& e)
    {
      this->set_error(e.what());
    }

  protected:
    LuaIntf::LuaContext m_ctx;

  private:
    const char* m_script_dir = "rom/";
    //const fw::random::Type m_random_type = fw::random::Type::GAME;

    LuaIntf::LuaState m_thread = nullptr;
    bool m_end_flag = true;
    bool m_error_flag = false;
    bool m_assert_flag = false; // falseだと止める
    const char* m_co_str; // コルーチン実行関数
  };

  class ScrSpawner {
  public:
  };

  class LuaEnemySpawner : public LuaScriptBase {
  public:
    LuaEnemySpawner()
      : LuaScriptBase("spawn_exec")
    {
      this->load_and_setup("level0.lua");
    }
    virtual ~LuaEnemySpawner() = default;
  private:
    virtual void bind() override {
 
    }
    virtual void update(float dt) override {
    }
  };
  // 敵生成用のlua script作成
  ILuaScript* create_lua_enemy_spawner()
  {
    return new LuaEnemySpawner();
  }
}