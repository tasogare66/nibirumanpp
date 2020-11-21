#include "pch.h"
#include <cmath>
#include "../src/FwFlag.h"
#include "../src/FwUtil.h"

TEST(UtilTest, FwFlagTest) {
  enum class TestFlag : uint32_t {
    FlagA = 1 << 0,
    FlagB = 1 << 1,
    FlagC = 1 << 2,
    FlagD = 1 << 3,
    FlagE = 1 << 4,

    FlagMaskA = (FlagB|FlagD),
  };

  FwFlag<TestFlag> flag0;
  EXPECT_EQ(flag0.none(), true);
  EXPECT_EQ(flag0.any(), false);
  flag0.set(TestFlag::FlagE);
  EXPECT_EQ(flag0.none(), false);
  EXPECT_EQ(flag0.any(), true);
  flag0.set(TestFlag::FlagB);
  EXPECT_EQ(flag0.value(), fw::underlying_cast(TestFlag::FlagE)| fw::underlying_cast(TestFlag::FlagB));
  flag0.reset();
  EXPECT_EQ(flag0.none(), true);

  flag0.set(TestFlag::FlagC, true);
  EXPECT_EQ(flag0.test(TestFlag::FlagC), true);
  EXPECT_EQ(flag0.value(), fw::underlying_cast(TestFlag::FlagC));
  flag0.set(TestFlag::FlagC, false);
  EXPECT_EQ(flag0.test(TestFlag::FlagC), false);
}