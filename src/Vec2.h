#pragma once
#include <cmath>

template <class T>
class Vec2 {
public:
  T x, y;

  Vec2() :x(0), y(0) {}
  Vec2(T x, T y) : x(x), y(y) {}
  Vec2(const Vec2& v) : x(v.x), y(v.y) {}

  Vec2& operator=(const Vec2& v) {
    x = v.x;
    y = v.y;
    return *this;
  }

  Vec2 operator+(Vec2& v) {
    return Vec2(x + v.x, y + v.y);
  }
  Vec2 operator-(Vec2& v) {
    return Vec2(x - v.x, y - v.y);
  }

  Vec2& operator+=(Vec2& v) {
    x += v.x;
    y += v.y;
    return *this;
  }
  Vec2& operator-=(Vec2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  vec2 operator+(double s) {
    return vec2(x + s, y + s);
  }
  vec2 operator-(double s) {
    return vec2(x - s, y - s);
  }
  vec2 operator*(double s) {
    return vec2(x * s, y * s);
  }
  vec2 operator/(double s) {
    return vec2(x / s, y / s);
  }

  vec2& operator+=(double s) {
    x += s;
    y += s;
    return *this;
  }
  vec2& operator-=(double s) {
    x -= s;
    y -= s;
    return *this;
  }
  vec2& operator*=(double s) {
    x *= s;
    y *= s;
    return *this;
  }
  vec2& operator/=(double s) {
    x /= s;
    y /= s;
    return *this;
  }

  void set(T x, T y) {
    this->x = x;
    this->y = y;
  }

  void rotate(double deg) {
    double theta = deg / 180.0 * M_PI;
    double c = cos(theta);
    double s = sin(theta);
    double tx = x * c - y * s;
    double ty = x * s + y * c;
    x = tx;
    y = ty;
  }

  vec2& normalize() {
    if (length() == 0) return *this;
    *this *= (1.0 / length());
    return *this;
  }

  float dist(vec2 v) const {
    vec2 d(v.x - x, v.y - y);
    return d.length();
  }
  float length() const {
    return std::sqrt(x * x + y * y);
  }

  static float dot(vec2 v1, vec2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
  }
  static float cross(vec2 v1, vec2 v2) {
    return (v1.x * v2.y) - (v1.y * v2.x);
  }

};

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
