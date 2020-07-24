#include "stdafx.h"

#include "App.h"

int main() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "nibiruman2080");
  auto app(std::make_unique<App>());

  sf::Texture texture;
  if (texture.create(10, 10)) {
    //error
  }
  sf::Uint8 pixels[10 * 10 * 4];// = new sf::Uint8[10 * 10 * 4];
  std::memset(pixels, 255,sizeof(pixels));
  texture.update(pixels);

  sf::Sprite sprite;
  sprite.setTexture(texture);
  sprite.setPosition(100,100);


  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    window.clear(sf::Color::Black);
    app->update();
    app->draw(window);

    window.draw(sprite);

    window.display();
  }
  return 0;
}