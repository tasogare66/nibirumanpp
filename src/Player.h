#pragma once
#include "Entity.h"
class Reticle;

class Player : public Entity {
public:
  explicit Player(const EntityArgs& args, const Reticle* reticle, int32_t index);
  virtual ~Player() = default;
  void init() override final;
  void update(float dt) override final;
  void draw(sf::RenderWindow& window) override final;
  float get_capradius() const { return m_capradius; }
  auto get_index() const { return m_index; } //player index

  static HitMask get_player_hit_mask(int32_t player_index) {
    FW_ASSERT(player_index >= 0 && player_index < Entity_PLAYER_MAX);
    return static_cast<HitMask>(fw::underlying_cast(HitMask::Player0) << player_index);
  }
  static HitMask get_generated_player_hit_mask(int32_t player_index) {
    FW_ASSERT(player_index >= 0 && player_index < Entity_PLAYER_MAX);
    return static_cast<HitMask>(fw::underlying_cast(HitMask::Generated_Pl0) << player_index);
  }

  auto get_product_colli_attr() const { return m_product_colli_attr; }
  void add_armslv();

private:
  void set_animdir(const Vec2f& in_dir);
  bool is_dash() const { return (m_dashst != 0); }
  bool is_dashing() const { return (m_dashst == 1); }
  float dash_pow() const {
    return 1.0f - std::max(m_dasht / m_dash_limit, 0.0f);
  }
  void reset_dash() {
    m_flwt = 0.0f;
    m_coolt = 0.0f;
  }
  bool check_dead();
  void set_invincible();
  void upd_invincible(float dt);
  void upd_armslv(float dt);

  const int32_t m_index; //player index
  int32_t m_shot_repeat = 0;
  float m_invincible_time = 0.f;
  float m_capradius = 20.0f;
  uint8_t m_armslv = 0;
  float m_armstime = 0.0f;
  const Reticle* m_reticle = nullptr;
  float m_elp = 0.0f;
  uint32_t m_animcnt = 0;
  uint32_t m_animdir = 0;
  bool m_active = true;
  HitMask m_product_colli_attr; //生成物に設定
  int m_dashst = 0;
  float m_dasht = 0.0f;
  float m_flwt = 0.0f;
  float m_coolt = 0.0f;
  Vec2f m_dashvec = {0.f,1.f};
  Vec2f m_dashvec_old;
  Vec2f m_chara_dir_old;
  static constexpr float m_dash_limit = 2.2f;
  static constexpr float m_dash_coolt = 4.2f;
  static constexpr float m_dash_followt = 0.9f;
  static constexpr float m_analog_threshold = 0.5f;
};