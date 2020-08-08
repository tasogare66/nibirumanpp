#pragma once
#include <SFML\Graphics.hpp>
#include <cmath>

template <typename T>
class Vec2 : public sf::Vector2<T> {
public:
  Vec2() : sf::Vector2<T>() {}
  Vec2(T ix, T iy) : sf::Vector2<T>(ix,iy) {}
  template <typename U>
  inline Vec2(const Vec2<U>& v) : sf::Vector2<T>(v.x, v.y) {}
  template <typename U>
  inline Vec2(const sf::Vector2<U>& v) : sf::Vector2<T>(v) {}

  void set(T ix, T iy) {
    this->x = ix;
    this->y = iy;
  }

  float sqr_magnitude() const {
    return this->x * this->x + this->y * this->y;
  }

  float magnitude() const {
    return std::sqrt(this->sqr_magnitude());
  }

  Vec2& normalize() {
    const auto len = this->magnitude();
    if (len == 0) {
      FW_ASSERT(0);
      return *this;
    }
    *this *= static_cast<T>(1.0 / len);
    return *this;
  }

  Vec2& set_rotate(float rad) {
    auto c = std::cos(rad);
    auto s = std::sin(rad);
    auto tx = this->x;
    auto ty = this->y;
    this->x = tx * c - ty * s;
    this->y = ty * c + tx * s;
    return *this;
  }

  Vec2 rotate(float rad) const {
    auto v = *this;
    return v.set_rotate(rad);
  }

  static float dot(const Vec2& v1, const Vec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
  }
  static float cross(const Vec2& v1, const Vec2& v2) {
    return (v1.x * v2.y) - (v1.y * v2.x);
  }
  static float angle(const Vec2& a, const Vec2& b) {
    auto am = a.magnitude();
    auto bm = b.magnitude();
    return std::acos(std::clamp(Vec2::dot(a, b) / (am * bm), -1.f, 1.f));
  }
};

template <typename T>
Vec2<T> operator -(const Vec2<T>& right);

template <typename T>
Vec2<T>& operator +=(Vec2<T>& left, const Vec2<T>& right);

template <typename T>
Vec2<T>& operator -=(Vec2<T>& left, const Vec2<T>& right);

template <typename T>
Vec2<T> operator +(const Vec2<T>& left, const Vec2<T>& right);

template <typename T>
Vec2<T> operator -(const Vec2<T>& left, const Vec2<T>& right);

template <typename T>
Vec2<T> operator *(const Vec2<T>& left, T right);

template <typename T>
Vec2<T> operator *(T left, const Vec2<T>& right);

template <typename T>
Vec2<T>& operator *=(Vec2<T>& left, T right);

template <typename T>
Vec2<T> operator /(const Vec2<T>& left, T right);

template <typename T>
Vec2<T>& operator /=(Vec2<T>& left, T right);

template <typename T>
bool operator ==(const Vec2<T>& left, const Vec2<T>& right);

template <typename T>
bool operator !=(const Vec2<T>& left, const Vec2<T>& right);

template <typename T>
inline Vec2<T> operator -(const Vec2<T>& right)
{
  return Vec2<T>(-right.x, -right.y);
}

template <typename T>
inline Vec2<T>& operator +=(Vec2<T>& left, const Vec2<T>& right)
{
  left.x += right.x;
  left.y += right.y;
  return left;
}

template <typename T>
inline Vec2<T>& operator -=(Vec2<T>& left, const Vec2<T>& right)
{
  left.x -= right.x;
  left.y -= right.y;
  return left;
}

template <typename T>
inline Vec2<T> operator +(const Vec2<T>& left, const Vec2<T>& right)
{
  return Vec2<T>(left.x + right.x, left.y + right.y);
}

template <typename T>
inline Vec2<T> operator -(const Vec2<T>& left, const Vec2<T>& right)
{
  return Vec2<T>(left.x - right.x, left.y - right.y);
}

template <typename T>
inline Vec2<T> operator *(const Vec2<T>& left, T right)
{
  return Vec2<T>(left.x * right, left.y * right);
}

template <typename T>
inline Vec2<T> operator *(T left, const Vec2<T>& right)
{
  return Vec2<T>(right.x * left, right.y * left);
}

template <typename T>
inline Vec2<T>& operator *=(Vec2<T>& left, T right)
{
  left.x *= right;
  left.y *= right;
  return left;
}

template <typename T>
inline Vec2<T> operator /(const Vec2<T>& left, T right)
{
  if (right == 0) {
    FW_ASSERT(0);
    right = 1;
  }
  return Vec2<T>(left.x / right, left.y / right);
}

template <typename T>
inline Vec2<T>& operator /=(Vec2<T>& left, T right)
{
  if (right == 0) {
    FW_ASSERT(0);
    right = 1;
  }
  left.x /= right;
  left.y /= right;
  return left;
}

template <typename T>
inline bool operator ==(const Vec2<T>& left, const Vec2<T>& right)
{
  return (left.x == right.x) && (left.y == right.y);
}

template <typename T>
inline bool operator !=(const Vec2<T>& left, const Vec2<T>& right)
{
  return (left.x != right.x) || (left.y != right.y);
}

using Vec2f = Vec2<float>;
