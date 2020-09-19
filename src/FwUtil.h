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
  T lerp(const T& a, const T& b, float t) {
    return a * (1 - t) + b * t;
  }

  //uderlying_cast
  namespace detail{
    template <typename E>
    using UnderlyingType = typename std::underlying_type<E>::type;

    template <typename E>
    using EnumTypesOnly = typename std::enable_if<std::is_enum<E>::value, E>::type;
  }
  template <typename E, typename = detail::EnumTypesOnly<E>>
  constexpr detail::UnderlyingType<E> underlying_cast(E e) {
    return static_cast<detail::UnderlyingType<E>>(e);
  }

  namespace util{
    std::string get_datetime_string();
  }
}