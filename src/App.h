#pragma once
class App {
public:
  App();
  ~App() = default;
  void update(float dt);
  void draw(sf::RenderWindow&);
private:
};