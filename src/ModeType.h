#pragma once
enum class ModeType : uint8_t {
  NONE = 0,
  TITLE,
  GAME,
};

class ModeTitle;
class ModeGame;

template<typename T>
struct ModeToType;

template<>
struct ModeToType<ModeTitle> { static constexpr ModeType type = ModeType::TITLE; };
template<>
struct ModeToType<ModeGame> { static constexpr ModeType type = ModeType::GAME; };
