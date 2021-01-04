#include "stdafx.h"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

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
    lambda_write_dst("health", dst.m_health);
    lambda_write_dst("score", dst.m_score);
    lambda_write_dst("radius", dst.m_radius);
    lambda_write_dst("mass", dst.m_mass);
    lambda_write_dst("exp_resi", dst.m_exp_resi);
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
