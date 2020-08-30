//----------------------------------------------------------------------------------------
//
//	Xoshiro-cpp
//	Xoshiro PRNG wrapper library for C++17 / C++20
//
//	Copyright (C) 2020 Ryo Suzuki <reputeless@gmail.com>
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files(the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions :
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
//
//----------------------------------------------------------------------------------------

#pragma once
#include <cstdint>
#include <array>
#include <limits>
#include <type_traits>
#if __has_cpp_attribute(nodiscard) >= 201907L
  #define XOSHIROCPP_NODISCARD_CXX20 [[nodiscard]]
#else
  #define XOSHIROCPP_NODISCARD_CXX20
#endif

namespace Xoshiro
{
  // A default seed value for the generators
  inline constexpr std::uint64_t DefaultSeed = 1234567890ULL;

  // Converts given uint32 value `i` into a 32-bit floating
  // point value in the range of [0.0f, 1.0f)
  template <class Uint32, std::enable_if_t<std::is_same_v<Uint32, std::uint32_t>>* = nullptr>
  [[nodiscard]]
  inline constexpr float FloatFromBits(Uint32 i) noexcept;

  // Converts given uint64 value `i` into a 64-bit floating
  // point value in the range of [0.0, 1.0)
  template <class Uint64, std::enable_if_t<std::is_same_v<Uint64, std::uint64_t>>* = nullptr>
  [[nodiscard]]
  inline constexpr double DoubleFromBits(Uint64 i) noexcept;

  // SplitMix64
  // Output: 64 bits
  // Period: 2^64
  // Footprint: 8 bytes
  // Original implementation: http://prng.di.unimi.it/splitmix64.c
  class SplitMix64
  {
  public:
    using state_type = std::uint64_t;
    using result_type = std::uint64_t;

    XOSHIROCPP_NODISCARD_CXX20
      explicit constexpr SplitMix64(state_type state = DefaultSeed) noexcept;

    constexpr result_type operator()() noexcept;

    template <size_t N>
    [[nodiscard]]
    constexpr std::array<std::uint64_t, N> generateSeedSequence() noexcept;

    [[nodiscard]]
    static constexpr result_type min() noexcept;

    [[nodiscard]]
    static constexpr result_type max() noexcept;

    [[nodiscard]]
    constexpr state_type serialize() const noexcept;

    constexpr void deserialize(state_type state) noexcept;

    [[nodiscard]]
    friend bool operator ==(const SplitMix64& lhs, const SplitMix64& rhs) noexcept
    {
      return (lhs.m_state == rhs.m_state);
    }

    [[nodiscard]]
    friend bool operator !=(const SplitMix64& lhs, const SplitMix64& rhs) noexcept
    {
      return (lhs.m_state != rhs.m_state);
    }

  private:
    state_type m_state;
  };

  // xoshiro256**
  // Output: 64 bits
  // Period: 2^256 - 1
  // Footprint: 32 bytes
  // Original implementation: http://prng.di.unimi.it/xoshiro256starstar.c
  // Version: 1.0
  class Xoshiro256StarStar
  {
  public:
    using state_type = std::array<std::uint64_t, 4>;
    using result_type = std::uint64_t;

    XOSHIROCPP_NODISCARD_CXX20
      explicit constexpr Xoshiro256StarStar(std::uint64_t seed = DefaultSeed) noexcept;

    XOSHIROCPP_NODISCARD_CXX20
      explicit constexpr Xoshiro256StarStar(state_type state) noexcept;

    constexpr result_type operator()() noexcept;

    // This is the jump function for the generator. It is equivalent
    // to 2^128 calls to next(); it can be used to generate 2^128
    // non-overlapping subsequences for parallel computations.
    constexpr void jump() noexcept;

    // This is the long-jump function for the generator. It is equivalent to
    // 2^192 calls to next(); it can be used to generate 2^64 starting points,
    // from each of which jump() will generate 2^64 non-overlapping
    // subsequences for parallel distributed computations.
    constexpr void longJump() noexcept;

    [[nodiscard]]
    static constexpr result_type min() noexcept;

    [[nodiscard]]
    static constexpr result_type max() noexcept;

    [[nodiscard]]
    constexpr state_type serialize() const noexcept;

    constexpr void deserialize(state_type state) noexcept;

    [[nodiscard]]
    friend bool operator ==(const Xoshiro256StarStar& lhs, const Xoshiro256StarStar& rhs) noexcept
    {
      return (lhs.m_state == rhs.m_state);
    }

    [[nodiscard]]
    friend bool operator !=(const Xoshiro256StarStar& lhs, const Xoshiro256StarStar& rhs) noexcept
    {
      return (lhs.m_state != rhs.m_state);
    }

  private:
    state_type m_state;
  };
}

////////////////////////////////////////////////////////////////

namespace Xoshiro
{
  template <class Uint32, std::enable_if_t<std::is_same_v<Uint32, std::uint32_t>>*>
  inline constexpr float FloatFromBits(const Uint32 i) noexcept
  {
    return (i >> 8) * 0x1.0p-24f;
  }

  template <class Uint64, std::enable_if_t<std::is_same_v<Uint64, std::uint64_t>>*>
  inline constexpr double DoubleFromBits(const Uint64 i) noexcept
  {
    return (i >> 11) * 0x1.0p-53;
  }

  namespace detail
  {
    [[nodiscard]]
    static constexpr std::uint64_t RotL(const std::uint64_t x, const int s) noexcept
    {
      return (x << s) | (x >> (64 - s));
    }

    [[nodiscard]]
    static constexpr std::uint32_t RotL(const std::uint32_t x, const int s) noexcept
    {
      return (x << s) | (x >> (32 - s));
    }
  }

  ////////////////////////////////////////////////////////////////
  //
  //	SplitMix64
  //
  inline constexpr SplitMix64::SplitMix64(const state_type state) noexcept
    : m_state(state) {}

  inline constexpr SplitMix64::result_type SplitMix64::operator()() noexcept
  {
    std::uint64_t z = (m_state += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
  }

  template <size_t N>
  inline constexpr std::array<std::uint64_t, N> SplitMix64::generateSeedSequence() noexcept
  {
    std::array<std::uint64_t, N> seeds = {};

    for (auto& seed : seeds)
    {
      seed = operator()();
    }

    return seeds;
  }

  inline constexpr SplitMix64::result_type SplitMix64::min() noexcept
  {
    return std::numeric_limits<result_type>::lowest();
  }

  inline constexpr SplitMix64::result_type SplitMix64::max() noexcept
  {
    return std::numeric_limits<result_type>::max();
  }

  inline constexpr SplitMix64::state_type SplitMix64::serialize() const noexcept
  {
    return m_state;
  }

  inline constexpr void SplitMix64::deserialize(const state_type state) noexcept
  {
    m_state = state;
  }

  ////////////////////////////////////////////////////////////////
  //
  //	xoshiro256**
  //
  inline constexpr Xoshiro256StarStar::Xoshiro256StarStar(const std::uint64_t seed) noexcept
    : m_state(SplitMix64{ seed }.generateSeedSequence<4>()) {}

  inline constexpr Xoshiro256StarStar::Xoshiro256StarStar(const state_type state) noexcept
    : m_state(state) {}

  inline constexpr Xoshiro256StarStar::result_type Xoshiro256StarStar::operator()() noexcept
  {
    const std::uint64_t result = detail::RotL(m_state[1] * 5, 7) * 9;
    const std::uint64_t t = m_state[1] << 17;
    m_state[2] ^= m_state[0];
    m_state[3] ^= m_state[1];
    m_state[1] ^= m_state[2];
    m_state[0] ^= m_state[3];
    m_state[2] ^= t;
    m_state[3] = detail::RotL(m_state[3], 45);
    return result;
  }

  inline constexpr void Xoshiro256StarStar::jump() noexcept
  {
    constexpr std::uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

    std::uint64_t s0 = 0;
    std::uint64_t s1 = 0;
    std::uint64_t s2 = 0;
    std::uint64_t s3 = 0;

    for (std::uint64_t jump : JUMP)
    {
      for (int b = 0; b < 64; ++b)
      {
        if (jump & UINT64_C(1) << b)
        {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }
        operator()();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }

  inline constexpr void Xoshiro256StarStar::longJump() noexcept
  {
    constexpr std::uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

    std::uint64_t s0 = 0;
    std::uint64_t s1 = 0;
    std::uint64_t s2 = 0;
    std::uint64_t s3 = 0;

    for (std::uint64_t jump : LONG_JUMP)
    {
      for (int b = 0; b < 64; ++b)
      {
        if (jump & UINT64_C(1) << b)
        {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }
        operator()();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }

  inline constexpr Xoshiro256StarStar::result_type Xoshiro256StarStar::min() noexcept
  {
    return std::numeric_limits<result_type>::lowest();
  }

  inline constexpr Xoshiro256StarStar::result_type Xoshiro256StarStar::max() noexcept
  {
    return std::numeric_limits<result_type>::max();
  }

  inline constexpr Xoshiro256StarStar::state_type Xoshiro256StarStar::serialize() const noexcept
  {
    return m_state;
  }

  inline constexpr void Xoshiro256StarStar::deserialize(const state_type state) noexcept
  {
    m_state = state;
  }
}