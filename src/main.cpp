#include "stdafx.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include "App.h"

#if DEBUG&0 //count number of memory allocations
uint64_t numOfHeapAllocations = 0;
void* operator new(size_t size)
{
  numOfHeapAllocations++;
  return malloc(size);
}
#endif

int main() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "nibiruman2080");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);
  ImGui::GetIO().IniFilename = "ram/imgui.ini";
  ImGui::GetIO().LogFilename = "ram/imgui_log.txt";

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
    ImGui::ShowDemoWindow();
    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();

    window.clear(sf::Color(0x44, 0x24, 0x34));
    {
      app->draw(window);
      ImGui::SFML::Render(window);
    }
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}