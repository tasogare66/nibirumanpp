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

const SeqPlayer* GameSeq::get_seq_player(uint32_t id) {
  if (id < m_seq_pls.size()) return &m_seq_pls[id];
  return nullptr;
}

const std::vector<Player*>& GameSeq::get_player_entities() {
  return m_pl_entities;
}

float GameSeq::getDifV(float a, float b) {
  return a;
}
