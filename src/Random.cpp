#include "stdafx.h"

#include <ctime>
#include "Xoshiro.hpp"

#include "Random.h"

namespace rng {
  class Random {
  public:
    Random() {
      auto s = std::time(nullptr);
      Xoshiro::Xoshiro256StarStar rng{ static_cast<uint64_t>(s) };
      for (auto& r : m_rngs) {
        r = Xoshiro::Xoshiro256StarStar{ rng() };
      }
    }
    ~Random() = default;
    auto& rng(Type t) { return m_rngs[static_cast<size_t>(t)]; }

	//[0.0-1.0)
	double rand(Type t) {
      return Xoshiro::DoubleFromBits(this->rng(t)());
	}

    //[0.0-range)
	float randf(float range, Type t) {
	  double d = this->rand(t);
      auto ret = d * static_cast<double>(range);
      return static_cast<float>(ret);
    }

    //[min-max)
    float rangef(float min, float max, Type t) {
      return this->randf(max - min, t) + min;
    }

    //[0-max]
    int32_t rand_int(int32_t max, Type t) {
      FW_ASSERT(max >= 0);
      double d = this->rand(t);
      uint64_t u = max + 1;
      return static_cast<int32_t>(d * u);
    }

	//[min-max]
	int32_t range_int(int32_t min, int32_t max, Type t) {
	  FW_ASSERT(max - min >= 0);
      double d = this->rand(t);
	  uint64_t u = max - min + 1;
	  return static_cast<int32_t>(d * u) + min;
	}

  private:
    std::array<Xoshiro::Xoshiro256StarStar, static_cast<size_t>(Type::MAX)> m_rngs;
  } g_random;

  float randf(float range, Type t) { return g_random.randf(range, t); }
  float range(float min, float max, Type t) { return g_random.rangef(min, max, t); }
  int32_t rand_int(int32_t max, Type t) { return g_random.rand_int(max, t); }
  int32_t range_int(int32_t min, int32_t max, Type t) { return g_random.range_int(min, max, t); }
}