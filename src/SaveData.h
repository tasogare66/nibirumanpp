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
  uint32_t m_version = m_save_data_version;
  //sound
  float m_vol_sfx = 0.75f;
  float m_vol_music = 0.75f;
};

class SaveDataMng : public Singleton<SaveDataMng> {
public:
  SaveDataMng() = default;
  virtual ~SaveDataMng() = default;
  void setup_at_boot();
  void read_save_data();
  void write_save_data() {
    this->write_save_data(m_data);
  }
  void apply_save_data();

  const SaveData& get_data() const { return m_data; }
  SaveData& get_data_w() { return m_data; }

  static constexpr std::string_view m_save_data_fname="ram/save_data.json";
private:
  void write_save_data(const SaveData data);
  SaveData m_data;
};