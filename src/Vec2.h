#pragma once
#include <SFML\Graphics.hpp>
#include <cmath>

template <class T>
class Vec2 : public sf::Vector2<T> {
public:
  Vec2() : Vector2<T>() {}
  Vec2(T x, T y) : Vector2<T>(x,y) {}
  Vec2(const Vec2& v) : Vector2<T>(v) {}

  Vec2 operator+(T s) {
    return Vec2(this->x + s, this->y + s);
  }
  Vec2 operator-(T s) {
    return Vec2(this->x - s, this->y - s);
  }
  Vec2 operator*(T s) {
    return Vec2(this->x * s, this->y * s);
  }
  Vec2 operator/(T s) {
    return Vec2(this->x / s, this->y / s);
  }

  Vec2& operator+=(T s) {
    this->x += s;
    this->y += s;
    return *this;
  }
  Vec2& operator-=(T s) {
    this->x -= s;
    this->y -= s;
    return *this;
  }
  Vec2& operator*=(T s) {
    this->x *= s;
    this->y *= s;
    return *this;
  }
  Vec2& operator/=(T s) {
    this->x /= s;
    this->y /= s;
    return *this;
  }

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
    if (len == 0) return *this;
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

using Vec2f = Vec2<float>;
