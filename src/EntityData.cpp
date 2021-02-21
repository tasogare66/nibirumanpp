#include "stdafx.h"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "EntityDef.h"

#include "EntityData.h"
using json = nlohmann::json;

void EntityDataMng::setup_at_boot()
{
  std::ifstream instream("rom/entity_data.json", std::ios::in);
  std::string content((std::istreambuf_iterator<char>(instream)), (std::istreambuf_iterator<char>()));
  const auto jd = json::parse(content.c_str());

  auto& tbl=jd["datas"];
  auto n = tbl.size();
  m_datas.clear(); //きれいにしておく
  m_datas.resize(n);
  FW_ASSERT(n == fw::underlying_cast(EntityDataId::Max));

  for (uint32_t i = 0; i < n;++i) {
    const auto& src(tbl[i]);
    auto& dst(m_datas[i]);
    auto lambda_write_dst = [&src](const char* key, auto& dst_member) {
      if (src.contains(key)) {
        dst_member = std::make_optional(src[key]);
      }
    };

    dst.m_name = src["name"];
    lambda_write_dst("have_dot", dst.m_have_dot);
    lambda_write_dst("ene_dead_sfx", dst.m_ene_dead_sfx);
    lambda_write_dst("no_damage", dst.m_no_damage);
    lambda_write_dst("health", dst.m_health);
    FW_ASSERT(!dst.m_health || (dst.m_health && dst.m_health.value()>0));
    lambda_write_dst("score", dst.m_score);
    lambda_write_dst("radius", dst.m_radius);
    lambda_write_dst("mass", dst.m_mass);
    lambda_write_dst("exp_resi", dst.m_exp_resi);
    lambda_write_dst("appear_sec", dst.m_appear_sec);
    lambda_write_dst("spr_ene", dst.m_spr_ene);
    lambda_write_dst("spr_num", dst.m_spr_num);
    lambda_write_dst("anim_wait_frame", dst.m_anim_wait_frame);
    //pri
    if (src.contains("pri")) {
      dst.m_draw_pri = std::make_optional(str2entit_draw_pri(src["pri"]));
    }
    dst.m_valid = true;
  }
}

const EntityData& EntityDataMng::get(EntityDataId id)
{
  static EntityData dummy;
  if (id == EntityDataId::None) return dummy;
  FW_ASSERT(id<EntityDataId::Max);
  return m_datas[fw::underlying_cast(id)];
}
