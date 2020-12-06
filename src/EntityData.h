﻿#pragma once
#include <optional>
#include "Singleton.h"

enum class EntityDataId : uint16_t {
  EneArrow,
  BossArrow,

  Max,
  None = 0xffff
};

class EntityData {
public:
  std::string m_name;
  std::optional<bool> m_have_dot;
  std::optional<int32_t> m_health;
  std::optional<uint32_t> m_score;
};

class EntityDataMng : public Singleton<EntityDataMng> {
public:
  EntityDataMng()
  {}
  virtual ~EntityDataMng() = default;
  void setup_at_boot();
  const EntityData& get(EntityDataId id);
private:
  std::vector<EntityData> m_datas;
};