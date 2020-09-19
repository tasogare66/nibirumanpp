#include "stdafx.h"
#include <array>
#include "FwUtil.h"

namespace fw {
namespace util {
  std::string get_datetime_string()
  {
    //現在時刻取得
    struct tm now;
    time_t tt = time(nullptr);
    localtime_s(&now, &tt);

    std::array<char, 32> buff;
    sprintf_s(buff.data(), buff.size(), "%04d-%02d-%02dT%02d%02d%02d",
      now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
      now.tm_hour, now.tm_min, now.tm_sec);

    return std::string(buff.data());
  }
}
}