#pragma once
#include "Singleton.h"

class DwGui : public Singleton<DwGui> {
public:
  DwGui() = default;
  ~DwGui() = default;
  void show_window();
  bool check_screenshot_req() {
    bool ret = m_screenshot_req;
    m_screenshot_req = false;
    return ret;
  }
private:
  void show_window_internal();
  bool m_screenshot_req = false;
};