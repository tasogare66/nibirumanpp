#pragma once
#include <array>
namespace const_param {
  inline constexpr float EPSILON = 1e-08f;
  inline constexpr float FRAME2SEC = 1.f / 60.f;
  inline const char* const GAME_VER = "v.0.9.0";
  inline constexpr float SCR_WIDTH = 240.f;
  inline constexpr float SCR_HEIGHT = 136.f;
  inline constexpr float LvRadius = 170.f;
  inline constexpr float LvRadDr2 = LvRadius / 1.4142f;
  inline constexpr float BOUNCE = 0.5f;
  inline constexpr float WND_WIDTH = 1280.f;
  inline constexpr float WND_HEIGHT = 720.f;
  inline constexpr uint32_t TXT_CHR_SIZE = 32+24;
  static const std::array<sf::Color, 16> TIC_PAL = {
    sf::Color(0x140C1CFF), sf::Color(0x442434FF), sf::Color(0x30346DFF), sf::Color(0x4A4E4DFF),
    sf::Color(0x854C30FF), sf::Color(0x346524FF), sf::Color(0xD04648FF), sf::Color(0x757161FF),
    sf::Color(0x597DCEFF), sf::Color(0xD27D2CFF), sf::Color(0x8595A1FF), sf::Color(0x6DAA2CFF),
    sf::Color(0xD2AA99FF), sf::Color(0x6DC2CAFF), sf::Color(0xDAD45EFF), sf::Color(0xDEEED6FF),
  };
  inline const sf::Color& ticcol(int32_t id) {
    FW_ASSERT(id>=0||id<TIC_PAL.size());
    return TIC_PAL[id];
  }
}