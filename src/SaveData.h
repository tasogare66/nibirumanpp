#pragma once
#include "Singleton.h"

class SaveData {
public:
  SaveData() = default;
  ~SaveData() = default;
  void set_vol_sfx(float v) { m_vol_sfx = v; }
  float get_vol_sfx() const { return m_vol_sfx; }
  void set_vol_music(float v) { m_vol_music = v; }
  float get_vol_music() const { return m_vol_music; }
  static constexpr uint32_t m_save_data_version = 0;
private:
  friend class SaveDataMng;
  friend class SaveDataUtil;
  uint32_t m_version = m_save_data_version;
  //sound
  float m_vol_sfx = 0.75f;
  float m_vol_music = 0.75f;
};
class DebConf {
public:
  DebConf() = default;
  ~DebConf() = default;
#if DEBUG
  bool m_invincible = false; //player無敵
  bool m_no_life_sub = false; //life減らさない
  bool m_bg_guide_disp = false; //stageのガイド表示
  bool m_no_boss_script = false; //boss停止
  bool m_boss_least_health = false; //boss最小体力
  bool m_no_spawn_script = false; //敵生成停止
  int m_spawn_item_no = 0; //生成する敵
#endif
};

class SaveDataMng : public Singleton<SaveDataMng> {
public:
  SaveDataMng() = default;
  virtual ~SaveDataMng() = default;
  void setup_at_boot();
  void read_save_data();
  void write_save_data() {
    this->write_save_data(m_data, m_deb_conf);
  }
  void apply_save_data();
  void reset_deb_conf() { m_deb_conf = DebConf{}; }

  static std::string_view get_save_data_fname();
  const SaveData& get_data() const { return m_data; }
  SaveData& get_data_w() { return m_data; }
  static const DebConf& deb_conf() { return SaveDataMng::inst().m_deb_conf; }
  static DebConf& deb_conf_w() { return SaveDataMng::inst().m_deb_conf; }

private:
  static void write_save_data(const SaveData& data, const DebConf& deb_conf);
  SaveData m_data;
  DebConf m_deb_conf; //for debug
};