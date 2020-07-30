#pragma once
class App {
public:
  App();
  ~App() = default;
  void update(float dt, sf::RenderWindow& window);
  void draw(sf::RenderWindow&);
private:
};