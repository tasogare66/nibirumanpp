#pragma once
#include "Entity.h"

class Player : public Entity {
public:
  explicit Player(const EntityArgs& args, const Entity* reticle, int32_t index);
  virtual ~Player() = default;
  void init() override final;
  void update(float dt) override final;
  void draw(sf::RenderWindow& window) override final;
  float get_capradius() const { return m_capradius; }
  auto get_index() const { return m_index; } //player index

  static constexpr int32_t m_player_max = 4;
  static HitMask get_player_hit_mask(int32_t player_index) {
    FW_ASSERT(player_index >= 0 && player_index < m_player_max);
    return static_cast<HitMask>(fw::underlying_cast(HitMask::Player0) << player_index);
  }
  static HitMask get_generated_player_hit_mask(int32_t player_index) {
    FW_ASSERT(player_index >= 0 && player_index < m_player_max);
    return static_cast<HitMask>(fw::underlying_cast(HitMask::Generated_Pl0) << player_index);
  }
  auto get_product_colli_attr() const { return m_product_colli_attr; }

private:
  bool is_dashing() const { return false; }

  const int32_t m_index; //player index
  int32_t m_shot_repeat = 0;
  float m_invincible_time = 0.f;
  float m_capradius = 20.0f;
  int32_t m_armslv = 0;
  const Entity* m_reticle = nullptr;
  float m_elp = 0.0f;
  uint32_t m_animcnt = 0;
  uint32_t m_animdir = 0;
  bool m_active = true;
  HitMask m_product_colli_attr; //¶¬•¨‚Éİ’è
};