#pragma once
enum EntityDrawPri : uint16_t {
  EntityDrawPri_Start = 0, //手前

  EntityDrawPri_Player = EntityDrawPri_Start,
  EntityDrawPri_EneBullet,
  EntityDrawPri_Default,

  EntityDrawPri_Max
};

inline EntityDrawPri str2entit_draw_pri(std::string_view str) {
  if (str == "Plaer") return EntityDrawPri_Player;
  else if (str == "EneBullet") return EntityDrawPri_EneBullet;
  else if (str == "Default") return EntityDrawPri_Default;
  FW_ASSERT(0);
  return EntityDrawPri_Default;
}