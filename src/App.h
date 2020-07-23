#pragma once
class App {
public:
  App();
  ~App() = default;
  void update();
  void draw(sf::RenderWindow&);
private:
};