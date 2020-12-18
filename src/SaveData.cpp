#include "stdafx.h"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "Sound.h"

#include "SaveData.h"
using json = nlohmann::json;

void SaveDataMng::setup_at_boot()
{
  this->read_save_data();
  this->apply_save_data();
}

void SaveDataMng::read_save_data()
{
  std::ifstream instream(m_save_data_fname.data(), std::ios::in);
  std::string content((std::istreambuf_iterator<char>(instream)), (std::istreambuf_iterator<char>()));
  const auto jd = json::parse(content.c_str());

  const auto& src(jd["dat"]);
  auto lambda_read_key = [&src](const char* key, auto& dst_member) {
    if (src.contains(key)) {
      dst_member = src[key];
    }
  };

  SaveData dst;
  lambda_read_key("vol_sfx", dst.m_vol_sfx);
  lambda_read_key("vol_music", dst.m_vol_music);

  //copy
  m_data = dst;
}

void SaveDataMng::apply_save_data()
{
  auto& snd = Sound::inst();
  snd.set_sfx_volume(m_data.m_vol_sfx);
  snd.set_music_volume(m_data.m_vol_music);
}

void SaveDataMng::write_save_data(const SaveData data)
{
  json jd;
  jd["version"] = data.m_save_data_version;
  jd["vol_sfx"] = data.m_vol_sfx;
  jd["vol_music"] = data.m_vol_music;

  json container;
  container["h"] = 0; //FIXME:  
  container["date"] = fw::util::get_datetime_string();
  container["dat"] = jd;

  std::ofstream outstream("ram/save_data.json", std::ios::out);
  outstream << container;
  outstream.close();
}
