#pragma once
#include "Singleton.h"

enum class ModeType : uint8_t {
  TITLE,
  GAME,
};

class Mode;
class ModeMng : public Singleton<ModeMng> {
public:
  ModeMng();
  virtual ~ModeMng();

  void request(ModeType req);

  bool update(float dt);
  void update_post();
  void draw0(sf::RenderWindow& window);
  void draw1(sf::RenderWindow& window);
private:
  void init();
  void dest();
  bool ctrl(float dt);
  std::unique_ptr<Mode> m_cur_work = nullptr;
  std::unique_ptr<Mode> m_req_work = nullptr;
};