#include "stdafx.h"

#include "Random.h"

#include "GameSeq.h"

void GameSeq::reset()
{
  m_pl_entities.clear();
  m_seq_pls.clear();
}

void GameSeq::add_player(Player* e) {
  m_pl_entities.push_back(e);
  m_seq_pls.emplace_back(e);
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

const Player* GameSeq::get_player_for_enemy()
{
  if (m_seq_pls.size() <= 0) {
    FW_ASSERT(0);
    return nullptr;
  }
  auto* seq_pl = this->get_seq_player((m_get_player_cnt++) % m_seq_pls.size());
  if (seq_pl) return seq_pl->get_player();
  return nullptr;
}

int32_t GameSeq::decide_target_index() const
{
  FW_ASSERT(!m_pl_entities.empty());
  return rng::rand_int(static_cast<int32_t>(m_pl_entities.size())-1,rng::Type::GAME);
}

const Player* GameSeq::get_player_entity(uint32_t player_index) const
{
  if (player_index < m_pl_entities.size()) return m_pl_entities[player_index];
  FW_ASSERT(0);
  return nullptr;
}

const std::vector<Player*>& GameSeq::get_player_entities() const {
  return m_pl_entities;
}

void GameSeq::add_score(uint32_t player_index, PlayerScore v)
{
  if (auto* player = inst().get_seq_player_w(player_index)) {
    player->add_score(v);
  }
}

void GameSeq::add_multiplier(uint32_t player_index)
{
  if (auto* player = inst().get_seq_player_w(player_index)) {
    player->add_multiplier();
  }
}

void GameSeq::reset_multiplier(uint32_t player_index)
{
  if (auto* player = inst().get_seq_player_w(player_index)) {
    player->reset_multiplier();
  }
}

int32_t GameSeq::decriment_life(uint32_t player_index)
{
  if (auto* player = inst().get_seq_player_w(player_index)) {
    return player->decriment_life();
  }
  return 0;
}

float GameSeq::getDifV(float a, float b) {
  return a;
}

bool GameSeq::check_game_over() const
{
  bool ret = true;
  for (const auto& seq_pl : m_seq_pls) {
    if (seq_pl.get_life() > 0) {
      ret = false;
      break;
    }
  }
  return ret;;
}
