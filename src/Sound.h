#pragma once
#include <array>
#include "Singleton.h"

enum class SndChannel : uint16_t {
  SFX0=0,
  SFX1,
  SFX2,
  SFX3,
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
private:
  auto& get_buf(SfxId id) { return m_snd_bufs[static_cast<size_t>(id)]; }
  auto& get_snd(SndChannel ch) { return m_snds[static_cast<size_t>(ch)]; }
  void play(SfxId id, SndChannel ch);
  std::array<sf::SoundBuffer, static_cast<size_t>(SfxId::MAX)> m_snd_bufs;
  std::array<sf::Sound, static_cast<size_t>(SndChannel::MAX)> m_snds;
};