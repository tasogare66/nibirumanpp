#include "stdafx.h"

// warning対処
#pragma warning( push )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4189 )

#include <lua.hpp>
#include <LuaIntf/LuaIntf.h>

#pragma warning( pop )

#include "Random.h"
#include "ObjLst.h"
#include "ConstParam.h"
#include "EnemyType.h"
#include "Enemy.h"
#include "GameSeq.h"
#include "Player.h"
#include "Human.h"
#include "Boss.h"

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
        m_dt = dt;
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

      LuaBinding(m_ctx.state()).beginModule("GAME")
        .addFunction("randomf", []() { return rng::randf(m_rng_type); })
        .addFunction("randomi", [](int max) { return rng::rand_int(max, m_rng_type); })
        .addProperty("dt", [this]() { return m_dt; })
        .addProperty("LvRadius", []() { return const_param::LvRadius; })
        .addFunction("decide_target_index", []() { return GameSeq::inst().decide_target_index(); })
        .addFunction("get_target_position", [](int id) {
          const auto* tgt = get_player_entity(id);
          auto p = tgt->get_pos();
          return std::make_tuple(p.x, p.y);
        })
        .endModule();
    }

    static const Player* get_player_entity(int32_t index) {
      return GameSeq::inst().get_player_entity(index);
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
    float m_dt = 0.0f;
    static constexpr auto m_rng_type = rng::Type::GAME;

    LuaIntf::LuaState m_thread = nullptr;
    bool m_end_flag = true;
    bool m_error_flag = false;
    bool m_assert_flag = false; // falseだと止める
    const char* m_co_str; // コルーチン実行関数
  };

  void spawn_base(EnemyType type, const EntityArgs& entity_args) {
    Enemy* ent = nullptr;
    switch (type) {
#undef ENEMY_TYPE_DECL
#define ENEMY_TYPE_DECL(_id,_cls)	case EnemyType::_id: ent = new _cls(entity_args); break;

#include "EnemyType.h"

#undef ENEMY_TYPE_DECL
      //case EnemyType::SNAKE:
      //  ent = new EneSnake(entity_args);
      //  break;
      //case EnemyType::GRUNT:
      //  ent = new EneGrunt(entity_args);
      //  break;
    default:
      FW_ASSERT(0);
      break;
    }
    if (ent) {
      ent->attr_spawned();
    }
  }

  class ScrSpawner {
  public:
    ScrSpawner() = default;
    ~ScrSpawner() = default;
    uint32_t get_spawn_num() const { //生存している敵の数
      return ObjLst::inst().get_spawn_num();
    }
    uint32_t get_spawn_ttl() const { //生成した敵の総数
      return ObjLst::inst().get_spawn_ttl();
    }
    void spawn(EnemyType type, LuaRef tbl) {
      EntityArgs entity_args;
      entity_args.m_pos.x = tbl["px"].value<float>();
      entity_args.m_pos.y = tbl["py"].value<float>();
      entity_args.m_dir.x = tbl["dirx"].value<float>();
      entity_args.m_dir.y = tbl["diry"].value<float>();
      entity_args.m_param0 = tbl["prm0"].value<float>();
      spawn_base(type, entity_args);
    }
  private:
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

      LuaBinding(m_ctx.state()).beginModule("EnemyType")
        .addConstant("SNAKE", EnemyType::SNAKE)
        .addConstant("GRUNT", EnemyType::GRUNT)
        .addConstant("HULK", EnemyType::HULK)
        .addConstant("ARROW", EnemyType::ARROW)
        .addConstant("ARROW2", EnemyType::ARROW2)
        .addConstant("SPHE", EnemyType::SPHE)
        .addConstant("HUMAN", EnemyType::HUMAN)
        .addConstant("BOSS_BABY", EnemyType::BOSS_BABY)
        .endModule();
      LuaIntf::LuaBinding(m_ctx.state()).beginClass<ScrSpawner>("ScrSpawner")
        .addConstructor(LUA_ARGS())
        .addFunction("spawn", &ScrSpawner::spawn, LUA_ARGS(EnemyType, LuaRef))
        .addProperty("num", &ScrSpawner::get_spawn_num)
        .addProperty("ttl", &ScrSpawner::get_spawn_ttl)
        .endClass();
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