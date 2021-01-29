#include "stdafx.h"

#include "Random.h"
#include "ObjLst.h"
#include "Player.h"
#include "SaveData.h"

#include "GameSeq.h"

void GameSeq::reset()
{
  m_pl_entities.clear();
  m_alive_pl_ids.clear();
  m_seq_pls.clear();
  m_get_player_cnt = 0;
  m_difficulty = 0.0f;
  m_ticcnt = 0;
  m_diffsub = m_diffsub_initial_val;
}

void GameSeq::add_player(Player* e) {
  m_pl_entities.push_back(e);
  m_alive_pl_ids.push_back(e->get_index());
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
  //生きているplayerから
  if (m_alive_pl_ids.size() <= 0) {
    FW_ASSERT(0);
    return nullptr;
  }
  auto pid = m_alive_pl_ids[ (m_get_player_cnt++) % m_alive_pl_ids.size() ];
  return this->get_player_entity(pid);
}

int32_t GameSeq::decide_target_index() const
{
  //生きているplayerから
  FW_ASSERT(!m_alive_pl_ids.empty());
  auto rnd = rng::rand_int(static_cast<int32_t>(m_alive_pl_ids.size()) - 1, rng::Type::GAME);
  return m_alive_pl_ids[rnd];
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
#if DEBUG
    if (SaveDataMng::deb_conf().m_no_life_sub) {
      return seqpl->get_life(); //life減らさない
    }
#endif
    return seqpl->decriment_life();
  }
  return -1;
}

float GameSeq::getDifV(float a, float b) {
  return fw::lerp(a, b, m_difficulty);
}

void GameSeq::update_alive_pl() {
  if (m_alive_pl_ids.size() > 1) { //最後のplayerは残す
    m_alive_pl_ids.erase(std::remove_if(m_alive_pl_ids.begin(), m_alive_pl_ids.end(), [this](const auto& pid)->bool {
      const auto* p = this->get_player_entity(pid);
      return p->is_gone();
    }), m_alive_pl_ids.end());
  }
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
