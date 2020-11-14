#include "stdafx.h"

#include "Sound.h"

void Sound::setup_at_boot()
{
  const char* se_names[] = {
    "sfx 2.wav",
    "sfx 3.wav",
    "sfx 4.wav",
    "sfx 5.wav",
    "sfx 6.wav",
    "sfx 7.wav",
    "sfx 8.wav",
    "sfx 9.wav",
    "sfx 10.wav",
    "sfx 11.wav",
  };
  static_assert(fw::array_size(se_names) == static_cast<size_t>(SfxId::MAX));
  char buf[128];
  for (int i = 0; i < m_snd_bufs.size(); ++i) {
    sprintf_s(buf, fw::array_size(buf), "rom/%s",se_names[i]);
    auto& sb = m_snd_bufs[i];
    if (!sb.loadFromFile(buf)) {
      FW_ASSERT(0);
    }
  }
  //volume
  this->apply_sfx_volume();
  this->apply_music_volume();
}

void Sound::set_sfx_volume(float in_vol)
{
  m_vol_sfx = in_vol;
  for (uint16_t i = fw::underlying_cast(SndChannel::SFX0); i < fw::underlying_cast(SndChannel::SFX_MAX);++i) {
    m_snds[i].setVolume(m_vol_sfx * 100.0f);
  }
}

void Sound::set_music_volume(float in_vol)
{
  m_vol_music = in_vol;
  for (uint16_t i = fw::underlying_cast(SndChannel::MUSIC0); i < fw::underlying_cast(SndChannel::MAX); ++i) {
    m_snds[i].setVolume(m_vol_music * 100.0f);
  }
}

void Sound::play(SfxId id, SndChannel ch)
{
  auto& snd = this->get_snd(ch);
  snd.setBuffer(this->get_buf(id));
  snd.play();
}
