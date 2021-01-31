#pragma once
class App {
public:
  App();
  ~App() = default;
  union Result {
    struct {
      bool m_terminate : 1; //true返すと終了
      bool m_reset : 1; //request reset
    };
    uint32_t ret = 0;
  };
  Result update(float dt, sf::RenderWindow& window);
  void draw(sf::RenderWindow&);
private:
  void update_app(float dt, sf::RenderWindow& window);
  bool m_dbg_pause = false;
};