#pragma once
#include "Singleton.h"
class Player;

using PlayerScore = uint64_t;
class SeqPlayer {
public:
  SeqPlayer(Player* e)
    : m_e(e)
  {}
  Player* get_player() { return m_e; }
  const Player* get_player() const { return m_e; }
  int32_t get_life() const { return m_life; }
  void add_score(PlayerScore v) {
    m_score += static_cast<PlayerScore>(std::floor(v * m_multiplier));
  }
  PlayerScore get_score() const { return m_score; }
private:
  Player* m_e = nullptr;
  int32_t m_life = 3;
  PlayerScore m_score = 0;
  float m_multiplier = 1.0f;
};

class GameSeq : public Singleton<GameSeq> {
public:
  GameSeq() = default;
  ~GameSeq() = default;
  void reset();
  void add_player(Player* e);
  const SeqPlayer* get_seq_player(uint32_t id) const;
  SeqPlayer* get_seq_player_w(uint32_t id);
  const Player* get_player_for_enemy();
  const std::vector<Player*>& get_player_entities();
  static void add_score(uint32_t player_index, PlayerScore v);
  float getDifV(float a, float b);
private:
  std::vector<Player*> m_pl_entities;
  std::vector<SeqPlayer> m_seq_pls;
  uint32_t m_get_player_cnt = 0;
};