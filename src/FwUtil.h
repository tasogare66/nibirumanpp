#pragma once
namespace fw {
  template<typename TYPE, std::size_t SIZE>
  std::size_t array_size(const TYPE(&)[SIZE]) {
    return SIZE;
  }

  template<typename T>
  T lerp(T a, T b, float t) {
    return a * (1 - t) + b * t;
  }
}