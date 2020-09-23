#include "stdafx.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include "App.h"
#include "ConstParam.h"
#include "DwGui.h"

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
  sf::RenderWindow window(sf::VideoMode(static_cast<uint32_t>(const_param::WND_WIDTH), static_cast<uint32_t>(const_param::WND_HEIGHT)), "nibiruman2080");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);
  ImGui::GetIO().IniFilename = "ram/imgui.ini";
  ImGui::GetIO().LogFilename = "ram/imgui_log.txt";
  int screeshot_state = 0;

  auto app(std::make_unique<App>());

  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    auto dt = deltaClock.restart();
#if DEBUG&0
    auto dtsec = 1.f / 60.f;
#else
    auto dtsec = dt.asSeconds();
#endif
    //update app
    app->update(dtsec, window);
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
    //capture
    if (DwGui::inst().check_screenshot_req()) {
      take_screenshot(window);
    }

    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}