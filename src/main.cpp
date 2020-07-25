#include "stdafx.h"

#include "App.h"


#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>


int main() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "nibiruman2080");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  auto app(std::make_unique<App>());

  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    //update app
    app->update();
    //imgui
    ImGui::SFML::Update(window, deltaClock.restart());
    ImGui::ShowDemoWindow();
    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();

    window.clear(sf::Color::Black);
    {
      app->draw(window);
      ImGui::SFML::Render(window);
    }
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}