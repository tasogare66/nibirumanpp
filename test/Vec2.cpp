#include "pch.h"
#include <cmath>
#include "../src/Vec2.h"

TEST(Vec2Test, Vec2fCalc) {
  Vec2f a(1,2);
  EXPECT_EQ(a.x, 1);
  EXPECT_EQ(a.y, 2);
  Vec2f b(3, 4);
  Vec2f c = a + b;
  EXPECT_EQ(c.x, 4);
  EXPECT_EQ(c.y, 6);
  c += a;
  EXPECT_EQ(c.x, 5);
  EXPECT_EQ(c.y, 8);
  Vec2f zero;
  EXPECT_EQ(zero.x, 0);
  EXPECT_EQ(zero.y, 0);
  Vec2f tmp(zero);
  EXPECT_EQ(tmp.x, 0);
  EXPECT_EQ(tmp.y, 0);
  tmp.set(10,100);
  EXPECT_EQ(tmp.x, 10);
  EXPECT_FLOAT_EQ(tmp.sqr_magnitude(), 10*10+100*100);
  EXPECT_FLOAT_EQ(tmp.magnitude(), std::sqrt(10.f * 10.f + 100.f * 100.f));
  auto n = a;
  n.normalize();
  EXPECT_FLOAT_EQ(n.x, 1.0f / std::sqrt(5.f));
  EXPECT_FLOAT_EQ(n.y, 2.0f / std::sqrt(5.f));

  EXPECT_FLOAT_EQ(Vec2f::dot(a,b), 11);
  EXPECT_FLOAT_EQ(Vec2f::cross(a, b), -2);

  Vec2f r(1.f, 0.f);
  float rad = static_cast<float>(M_PI / 180 * 30);
  auto r30 = r.rotate(rad);
  EXPECT_FLOAT_EQ(Vec2f::angle(Vec2f(1.0f, 0.f), r30), rad);

  EXPECT_FLOAT_EQ(Vec2f::angle(Vec2f(1.f,0.f), Vec2f(1.f/std::sqrt(2.f),-1.f/std::sqrt(2.f))), static_cast<float>(M_PI/4.f));
}