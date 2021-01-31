#include "stdafx.h"
#include <array>

#include "Singleton.h"

namespace singleton {
  std::array<SingletonsAdapter::ResetFunc, 64> g_adapter_resets={};
  size_t g_adapter_rest_size = 0;
  inline void add_reset_func(SingletonsAdapter::ResetFunc func) {
    FW_ASSERT(g_adapter_rest_size < g_adapter_resets.size());
    g_adapter_resets[g_adapter_rest_size] = func;
    ++g_adapter_rest_size;
  }
  inline void invoke_resets() {
    //逆順に実行
    for (size_t i = g_adapter_rest_size; i > 0;) {
      --i;
      (*g_adapter_resets[i])();
    }
    //instance再構成する,g_adapter_rest_size はclearしない
  }
}

void SingletonsAdapter::add_reset_func(ResetFunc func)
{
  singleton::add_reset_func(func);
}
void SingletonsAdapter::invoke_resets()
{
  singleton::invoke_resets();
}
