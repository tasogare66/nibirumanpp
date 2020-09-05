#pragma once
#include "Singleton.h"
class Player;

class SeqPlayer {
public:
  SeqPlayer(Player* e)
    : m_e(e)
  {}
  Player* get_player() { return m_e; }
  const Player* get_player() const { return m_e; }
private:
  Player* m_e = nullptr;
  int32_t m_life = 3;
  uint64_t m_score = 0;
};

class GameSeq : public Singleton<GameSeq> {
public:
  GameSeq() = default;
  ~GameSeq() = default;
  void reset();
  void add_player(Player* e);
  const SeqPlayer* get_seq_player(uint32_t id) const;
  const Player* get_player_for_enemy();
  const std::vector<Player*>& get_player_entities();
  float getDifV(float a, float b);
private:
  std::vector<Player*> m_pl_entities;
  std::vector<SeqPlayer> m_seq_pls;
  uint32_t m_get_player_cnt = 0;
};