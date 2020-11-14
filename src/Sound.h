#pragma once
#include <array>
#include "Singleton.h"

enum class SndChannel : uint16_t {
  SFX0=0,
  SFX1,
  SFX2,
  SFX3,
  SFX_MAX,

  MUSIC0=SFX_MAX,
  MAX
};
enum class SfxId{
  EneDead, //2
  PlShot, //3
  Dot, //4
  Force, //5
  Human, //6
  MenuDecide, //7
  Dash, //8
  DashCoolt, //9
  EneBlink, //10
  BossAp, //11
  MAX
};

class Sound : public Singleton<Sound> {
public:
  Sound()
  {}
  virtual ~Sound() = default;
  void setup_at_boot();
  static void psfx(SfxId id, SndChannel ch) { inst().play(id, ch); }
  void set_sfx_volume(float in_vol);
  void apply_sfx_volume() { this->set_sfx_volume(m_vol_sfx); }
  float get_sfx_volume() const { return m_vol_sfx; }
  float* get_sfx_volume_pt() { return &m_vol_sfx; }
  void set_music_volume(float in_vol);
  void apply_music_volume() { this->set_music_volume(m_vol_music); }
  float get_music_volume() const { return m_vol_music; }
  float* get_music_volume_pt() { return &m_vol_music; }
private:
  auto& get_buf(SfxId id) { return m_snd_bufs[static_cast<size_t>(id)]; }
  auto& get_snd(SndChannel ch) { return m_snds[static_cast<size_t>(ch)]; }
  void play(SfxId id, SndChannel ch);
  std::array<sf::SoundBuffer, static_cast<size_t>(SfxId::MAX)> m_snd_bufs;
  std::array<sf::Sound, static_cast<size_t>(SndChannel::MAX)> m_snds;
  float m_vol_sfx = 0.8f; //[0-1]
  float m_vol_music = 0.8f; //[0-1]
};