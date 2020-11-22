﻿#include "stdafx.h"

#include "Random.h"
#include "ObjLst.h"
#include "Player.h"

#include "GameSeq.h"

void GameSeq::reset()
{
  m_pl_entities.clear();
  m_seq_pls.clear();
  m_get_player_cnt = 0;
  m_difficulty = 0.0f;
  m_ticcnt = 0;
  m_diffsub = m_diffsub_initial_val;
}

void GameSeq::add_player(Player* e) {
  m_pl_entities.push_back(e);
}

void GameSeq::setup_game()
{
  m_seq_pls.emplace_back();
  if (m_entry_num >= 2) {
    m_seq_pls[0].setup_for_coop();
  }
}

const SeqPlayer* GameSeq::get_seq_player(uint32_t id) const {
  if (id < m_seq_pls.size()) return &m_seq_pls[id];
  FW_ASSERT(0);
  return nullptr;
}

SeqPlayer* GameSeq::get_seq_player_w(uint32_t id)
{
  if (id < m_seq_pls.size()) return &m_seq_pls[id];
  FW_ASSERT(0);
  return nullptr;
}

bool GameSeq::is_exist_seq_player(uint32_t id) const
{
  return (id < m_seq_pls.size());
}

const Player* GameSeq::get_player_for_enemy()
{
  if (m_pl_entities.size() <= 0) {
    FW_ASSERT(0);
    return nullptr;
  }
  return this->get_player_entity((m_get_player_cnt++) % m_pl_entities.size());
}

int32_t GameSeq::decide_target_index() const
{
  FW_ASSERT(!m_pl_entities.empty());
  return rng::rand_int(static_cast<int32_t>(m_pl_entities.size())-1,rng::Type::GAME);
}

Player* GameSeq::get_player_entity_w(uint32_t player_index) const
{
  if (player_index < m_pl_entities.size()) return m_pl_entities[player_index];
  FW_ASSERT(0);
  return nullptr;
}

const Player* GameSeq::get_player_entity(uint32_t player_index) const {
  return this->get_player_entity_w(player_index);
}

const std::vector<Player*>& GameSeq::get_player_entities() const {
  return m_pl_entities;
}

void GameSeq::add_score(PlayerScore v)
{
  constexpr uint32_t player_index = 0;
  if (auto* seqpl = inst().get_seq_player_w(player_index)) {
    seqpl->add_score(v);
  }
}

void GameSeq::add_multiplier()
{
  constexpr uint32_t player_index = 0;
  if (auto* seqpl = inst().get_seq_player_w(player_index)) {
    seqpl->add_multiplier();
  }
}

void GameSeq::reset_multiplier()
{
  constexpr uint32_t player_index = 0;
  if (auto* seqpl = inst().get_seq_player_w(player_index)) {
    seqpl->reset_multiplier();
  }
}

int32_t GameSeq::decriment_life()
{
  constexpr uint32_t player_index = 0;
  if (auto* seqpl = inst().get_seq_player_w(player_index)) {
    return seqpl->decriment_life();
  }
  return -1;
}

float GameSeq::getDifV(float a, float b) {
  return a;
}

bool GameSeq::check_game_over() const
{
  //全playerが死亡だと終わり
  bool ret = true;
  for (const auto* pl : m_pl_entities) {
    ret &= pl->is_gone();
  }
  return ret;
}

void GameSeq::update_info(float dt)
{
  //multiplier
  for (auto& seq_pl : m_seq_pls) {
    seq_pl.update_info(dt);
  }
  //difficulty
  if (m_ticcnt % 30 == 0) {
    const auto spawn_ttl = ObjLst::inst().get_spawn_ttl();
    m_difficulty = std::min(std::max<int32_t>(spawn_ttl - m_diffsub, 0) / 2000.0f, 1.0f);
  }
  ++m_ticcnt;
}

void GameSeq::update_hiscore()
{
  for (const auto& seq_pl : m_seq_pls) {
    m_hiscore = std::max(m_hiscore, seq_pl.get_score());
  }
}
