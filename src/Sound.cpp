﻿#include "stdafx.h"

#include "Sound.h"

void Sound::setup_at_boot()
{
  const char* se_names[] = {
    "sfx 2.wav",
    "sfx 3.wav",
    "sfx 4.wav",
    "sfx 5.wav",
    "sfx 10.wav",
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
}

void Sound::play(SfxId id, SndChannel ch)
{
  auto& snd = this->get_snd(ch);
  snd.setBuffer(this->get_buf(id));
  snd.play();
}
