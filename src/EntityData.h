﻿#pragma once
#include <optional>
#include "Singleton.h"
enum EntityDrawPri : uint16_t;

enum class EntityDataId : uint16_t {
  EneSnake,
  EneUrchin,
  EneArrow,
  BossArrow,
  BossBullet,
  BossUrchin,
  BossUrchinNode,
  BossCobra,
  CobraHeadParts,
  CobraBodyParts,
  BossCog, //BossCog
  CogParts,
  CogTopParts,

  Max,
  None = 0xffff
};

class EntityData {
public:
  bool m_valid = false;
  std::string m_name;
  std::optional<bool> m_have_dot;
  std::optional<bool> m_ene_dead_sfx;
  std::optional<bool> m_no_damage; //EntityFlag::NoDamage,healthの引き算しない
  std::optional<int32_t> m_health;
  std::optional<uint32_t> m_score;
  std::optional<float> m_radius;
  std::optional<float> m_mass;
  std::optional<float> m_exp_resi;
  std::optional<float> m_appear_sec;
  std::optional<uint32_t> m_spr_ene;
  std::optional<uint16_t> m_spr_num;
  std::optional<float> m_anim_wait_frame;
  std::optional<EntityDrawPri> m_draw_pri;
  bool is_valid() const { return m_valid; }
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