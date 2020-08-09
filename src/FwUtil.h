#pragma once
namespace fw {
  template<typename TYPE, std::size_t SIZE>
  std::size_t array_size(const TYPE(&)[SIZE]) {
    return SIZE;
  }

  static constexpr float DEG2RAD = static_cast<float>(M_PI / 180);
  static constexpr float RAD2DEG = static_cast<float>(180 / M_PI);
  inline float deg2rad(float deg) {
    return deg * DEG2RAD;
  }
  inline float rad2deg(float rad) {
    return rad * RAD2DEG;
  }

  template<typename T>
  T lerp(T a, T b, float t) {
    return a * (1 - t) + b * t;
  }
}