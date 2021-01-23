#pragma once
#include "Singleton.h"

#if DEBUG
class DwGui : public Singleton<DwGui> {
public:
  DwGui() = default;
  ~DwGui() = default;
  void show_window();
  [[nodiscard]] bool check_screenshot_req() {
    bool ret = m_screenshot_req;
    m_screenshot_req = false;
    return ret;
  }
  [[nodiscard]] bool get_disable_spawn_scritp() const;
  [[nodiscard]] int check_spawn_req();
private:
  void show_window_internal();
  void show_options_window();
  bool m_screenshot_req = false;
  bool m_spawn_item_req = false;
};
#endif