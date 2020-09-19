#pragma once
enum class ModeType : uint8_t {
  NONE = 0,
  TITLE,
  GAME,
};

template<class T>
struct ModeToType {
  static ModeType type;
};
class ModeGame;
template<> ModeType ModeToType<ModeGame>::type = ModeType::GAME;
