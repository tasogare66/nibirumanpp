#include "pch.h"
#include <cmath>
#include "../src/Xoshiro.hpp"

TEST(RandTest, Xorshiro) {
  using namespace Xoshiro;

  const std::uint64_t seed = 12345;

  Xoshiro256StarStar rng(seed);
  EXPECT_EQ(rng(), 13720838825685603483);
  EXPECT_EQ(rng(), 2398916695208396998);
  EXPECT_EQ(rng(), 17770384849984869256);
  EXPECT_EQ(rng(), 891717726879801395);
  EXPECT_EQ(rng(), 10241316046318454344);
  auto seri = rng.serialize();
  auto v = rng();
  for (int i = 0; i < 10; ++i) rng();
  rng.deserialize(seri);
  EXPECT_EQ(rng(), v);
}