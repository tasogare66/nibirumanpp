#include "stdafx.h"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "Sound.h"

#include "SaveData.h"
using json = nlohmann::json;

template<typename T>
inline void read_key_function(const json& src, const char* key, T& dst_member) {
  if (src.contains(key)) {
    dst_member = src[key];
  }
}
#define SAVE_DATA_READ_KEY(_key) lambda_read_key(#_key, dst.m_ ## _key)
#define SAVE_DATA_WRITE_KEY(_key) jd[#_key] = (src.m_ ## _key)

class SaveDataUtil {
public:
  static SaveData deserialize_save_data(const json& src) {
    auto lambda_read_key = [&src](const char* key, auto& dst_member) {
      read_key_function(src, key, dst_member);
    };
    SaveData dst;
    SAVE_DATA_READ_KEY(vol_sfx);
    SAVE_DATA_READ_KEY(vol_music);
    return dst;
  }
  static json serialize_save_data(const SaveData& src) {
    json jd;
    SAVE_DATA_WRITE_KEY(vol_sfx);
    SAVE_DATA_WRITE_KEY(vol_music);
    return jd;
  }
#if DEBUG
  static DebConf deserialize_deb_conf(const json& src) {
    auto lambda_read_key = [&src](const char* key, auto& dst_member) {
      read_key_function(src, key, dst_member);
    };
    DebConf dst;
    SAVE_DATA_READ_KEY(fix_frame_time);
    SAVE_DATA_READ_KEY(invincible);
    SAVE_DATA_READ_KEY(no_life_sub);
    SAVE_DATA_READ_KEY(bg_guide_disp);
    SAVE_DATA_READ_KEY(no_boss_script);
    SAVE_DATA_READ_KEY(boss_least_health);
    SAVE_DATA_READ_KEY(no_spawn_script);
    SAVE_DATA_READ_KEY(spawn_item_no);
    return dst;
  }
  static json serialize_db_conf(const DebConf& src) {
    json jd;
    SAVE_DATA_WRITE_KEY(fix_frame_time);
    SAVE_DATA_WRITE_KEY(invincible);
    SAVE_DATA_WRITE_KEY(no_life_sub);
    SAVE_DATA_WRITE_KEY(bg_guide_disp);
    SAVE_DATA_WRITE_KEY(no_boss_script);
    SAVE_DATA_WRITE_KEY(boss_least_health);
    SAVE_DATA_WRITE_KEY(no_spawn_script);
    SAVE_DATA_WRITE_KEY(spawn_item_no);
    return jd;
  }
#endif
};

std::string_view SaveDataMng::get_save_data_fname()
{
#if DEBUG
  static constexpr std::string_view save_data_fname = "ram/save_data.dev.json";
#else
  static constexpr std::string_view save_data_fname = "ram/save_data.json";
#endif
  return save_data_fname;
}

void SaveDataMng::setup_at_boot()
{
  this->read_save_data();
  this->apply_save_data();
}

void SaveDataMng::read_save_data()
{
  std::ifstream instream(SaveDataMng::get_save_data_fname().data(), std::ios::in);
  if (!instream.is_open()) return;
  std::string content((std::istreambuf_iterator<char>(instream)), (std::istreambuf_iterator<char>()));
  const auto jd = json::parse(content.c_str());

  const auto& src(jd["dat"]);
  m_data = SaveDataUtil::deserialize_save_data(src);

#if DEBUG
  if (src.contains("deb_conf")) {
    m_deb_conf = SaveDataUtil::deserialize_deb_conf(src["deb_conf"]);
  }
#endif
}

void SaveDataMng::apply_save_data()
{
  auto& snd = Sound::inst();
  snd.set_sfx_volume(m_data.m_vol_sfx);
  snd.set_music_volume(m_data.m_vol_music);
}

void SaveDataMng::write_save_data(const SaveData& data, const DebConf& deb_conf)
{
  json jd = SaveDataUtil::serialize_save_data(data);
  jd["version"] = data.m_save_data_version;

#if DEBUG
  jd["deb_conf"] = SaveDataUtil::serialize_db_conf(deb_conf);
#endif

  json container;
  container["h"] = 0; //FIXME:  
  container["date"] = fw::util::get_datetime_string();
  container["dat"] = jd;

  std::ofstream outstream(SaveDataMng::get_save_data_fname().data(), std::ios::out);
  outstream << container;
  outstream.close();
}
