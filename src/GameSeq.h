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
  int32_t decriment_life() {
    m_life = std::max(m_life - 1, 0);
    return m_life;
  }
  int32_t get_life() const { return m_life; }
  void add_score(PlayerScore v) {
    m_score += static_cast<PlayerScore>(std::floor(v * m_multiplier));
  }
  PlayerScore get_score() const { return m_score; }
  void add_multiplier() {
    m_multiplier = std::min(m_multiplier + 0.01f, 15.f);
    m_multime = m_multimeLimit;
  }
  void reset_multiplier() {
    m_multiplier = 1.0;
    m_multime = 0.0f;
  }
private:
  Player* m_e = nullptr;
  int32_t m_life = 3;
  PlayerScore m_score = 0;
  float m_multiplier = 1.0f;
  float m_multime = 0.0f;
  static constexpr float m_multimeLimit = 6.0f;
};

class GameSeq : public Singleton<GameSeq> {
public:
  GameSeq() = default;
  ~GameSeq() = default;
  //menu
  void set_entry_num(uint32_t num) { FW_ASSERT(num<=2); m_entry_num = num; }
  uint32_t get_entry_num() const { return m_entry_num; }
  //game
  void reset();
  void add_player(Player* e);
  const SeqPlayer* get_seq_player(uint32_t id) const;
  SeqPlayer* get_seq_player_w(uint32_t id);
  const Player* get_player_for_enemy();
  int32_t decide_target_index() const; //spawn時のtarget(player)の指定,random
  Player* get_player_entity_w(uint32_t player_index) const;
  const Player* get_player_entity(uint32_t player_index) const;
  const std::vector<Player*>& get_player_entities() const;
  static void add_score(uint32_t player_index, PlayerScore v);
  static void add_multiplier(uint32_t player_index);
  static void reset_multiplier(uint32_t player_index);
  static int32_t decriment_life(uint32_t player_index);
  float getDifV(float a, float b);
  void  reduceDiff(int32_t v) { FW_ASSERT(v>0); m_diffsub += std::max(v,0); }
  bool check_game_over() const;
  PlayerScore get_hiscore() const { return m_hiscore; }
  void update_hiscore();
private:
  uint32_t m_entry_num = 1;
  //game
  std::vector<Player*> m_pl_entities;
  std::vector<SeqPlayer> m_seq_pls;
  uint32_t m_get_player_cnt = 0;
  float m_difficulty = 0.0f;
  uint64_t m_ticcnt = 0;
  int32_t m_diffsub = 500;
  PlayerScore m_hiscore = 5000;
};