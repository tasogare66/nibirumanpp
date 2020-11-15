#pragma once
class App {
public:
  App();
  ~App() = default;
  bool update(float dt, sf::RenderWindow& window); //false返すと終了
  void draw(sf::RenderWindow&);
private:
  void update_app(float dt, sf::RenderWindow& window);
  bool m_dbg_pause = false;
};