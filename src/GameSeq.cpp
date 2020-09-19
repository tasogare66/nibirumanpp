#include "stdafx.h"

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

const std::vector<Player*>& GameSeq::get_player_entities() {
  return m_pl_entities;
}

void GameSeq::add_score(uint32_t player_index, PlayerScore v)
{
  if (auto* player = inst().get_seq_player_w(player_index)) {
    player->add_score(v);
  }
}

float GameSeq::getDifV(float a, float b) {
  return a;
}
