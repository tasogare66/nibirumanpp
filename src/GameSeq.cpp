#include "stdafx.h"

#include "Random.h"

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
  m_seq_pls.emplace_back();
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
  uint32_t player_index = 0;
  if (auto* seqpl = inst().get_seq_player_w(player_index)) {
    seqpl->add_score(v);
  }
}

void GameSeq::add_multiplier()
{
  uint32_t player_index = 0;
  if (auto* seqpl = inst().get_seq_player_w(player_index)) {
    seqpl->add_multiplier();
  }
}

void GameSeq::reset_multiplier()
{
  uint32_t player_index = 0;
  if (auto* seqpl = inst().get_seq_player_w(player_index)) {
    seqpl->reset_multiplier();
  }
}

int32_t GameSeq::decriment_life()
{
  uint32_t player_index = 0;
  if (auto* seqpl = inst().get_seq_player_w(player_index)) {
    return seqpl->decriment_life();
  }
  return 0;
}

float GameSeq::getDifV(float a, float b) {
  return a;
}

bool GameSeq::check_game_over() const
{
  uint32_t player_index = 0;
  if (auto* seqpl = inst().get_seq_player_w(player_index)) {
    return (seqpl->get_life() <= 0);
  }
  return true; //ない場合game over
  //bool ret = true;
  //for (const auto& seq_pl : m_seq_pls) {
  //  if (seq_pl.get_life() > 0) {
  //    ret = false;
  //    break;
  //  }
  //}
  //return ret;;
}

void GameSeq::update_hiscore()
{
  for (const auto& seq_pl : m_seq_pls) {
    m_hiscore = std::max(m_hiscore, seq_pl.get_score());
  }
}
