﻿#include "stdafx.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include "App.h"
#include "ConstParam.h"
#include "DwGui.h"

#include <xmmintrin.h>
#define TRAP_FP_ERROR (DEBUG & 1)

#if DEBUG&0 //count number of memory allocations
uint64_t numOfHeapAllocations = 0;
void* operator new(size_t size) {
  numOfHeapAllocations++;
  return malloc(size);
}
uint64_t GetNumOfHeapAllocations() {
  return numOfHeapAllocations;
}
#endif

void setup_trap_fp_error()
{
  _mm_setcsr(_mm_getcsr() | _MM_FLUSH_ZERO_ON | _MM_MASK_UNDERFLOW);

#if TRAP_FP_ERROR
  constexpr unsigned int exception_mask = _MCW_EM;
  unsigned int new_control = exception_mask;
  new_control &= ~_EM_INVALID;
  new_control &= ~_EM_ZERODIVIDE;
  _controlfp_s(nullptr, new_control, exception_mask);
#endif
}

void take_screenshot(const sf::RenderWindow& window)
{
  std::string filename = "ram/" + fw::util::get_datetime_string() + ".png";
  sf::Texture texture;
  texture.create(window.getSize().x, window.getSize().y);
  texture.update(window);
  if (texture.copyToImage().saveToFile(filename)) {
    //std::cout << "screenshot saved to " << filename << std::endl;
  } else {
    FW_ASSERT(0);
  }
}

int main() {
  //メモリリーク検出
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  setup_trap_fp_error();

  sf::RenderWindow window(sf::VideoMode(static_cast<uint32_t>(const_param::WND_WIDTH), static_cast<uint32_t>(const_param::WND_HEIGHT)), "nibiruman2080");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);
  ImGui::GetIO().IniFilename = "ram/imgui.ini";
  ImGui::GetIO().LogFilename = "ram/imgui_log.txt";
  int screeshot_state = 0;

  auto app(std::make_unique<App>());

  bool app_exec = true;
  bool app_reset = false;
  sf::Clock deltaClock;
  while (window.isOpen() && app_exec) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    auto dt = deltaClock.restart();
    //update app
    auto app_result = app->update(dt.asSeconds(), window);
    app_exec = !app_result.m_terminate;
    //imgui
    ImGui::SFML::Update(window, dt);
#if DEBUG
    ImGui::ShowDemoWindow();
    DwGui::inst().show_window();
#endif

    window.clear(sf::Color(0x44, 0x24, 0x34));
    {
      app->draw(window);
      ImGui::SFML::Render(window);
    }
#if DEBUG
    //capture
    if (DwGui::inst().check_screenshot_req()) {
      take_screenshot(window);
    }
#endif
    window.display();

#if DEBUG
    if (app_result.m_reset) { //reset
      SingletonsAdapter::invoke_resets();
      app = std::make_unique<App>();
    }
#endif
  }

  ImGui::SFML::Shutdown();

  return 0;
}