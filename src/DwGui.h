#pragma once
#include "Singleton.h"

#if DEBUG
class DwGui : public Singleton<DwGui> {
public:
  DwGui() = default;
  ~DwGui() = default;
  void show_window();
private:
  void show_window_internal();
};
#endif