#pragma once
namespace rng{
  enum class Type : uint8_t {
    COMMON,
    GAME,
    EFFECT,
    MAX
  };
  float randf(Type t = Type::COMMON);
  float randf(float range, Type t = Type::COMMON);
  float range(float min, float max, Type t = Type::COMMON);
  int32_t rand_int(int32_t max, Type t = Type::COMMON);
  int32_t range_int(int32_t min, int32_t max, Type t = Type::COMMON);
}